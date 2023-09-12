#include "Components/CTargetComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "particles/ParticleSystem.h"
#include "particles/ParticleSystemComponent.h"

UCTargetComponent::UCTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CHelpers::GetAsset<UParticleSystem>(&Particle, "ParticleSystem'/Game/Effects/P_Enrage_Base.P_Enrage_Base'");


}


void UCTargetComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	
}


void UCTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckNull(Target);
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(Target);

	bool b = false;
	b |= state->IsDeadMode();
	// Target이 죽었다면, true 바꿔서 EndTargeting 하기 
	b |= Target->GetDistanceTo(OwnerCharacter) >= TraceRadius;
	// Target의 거리가 TraceRadius 보다 크게 된다면 true 바꿔서 EndTargeting 하기 
	// 즉 타겟이 안잡히는 조건들 설정

	if (b == true)
	{
		EndTargeting();

		return;
	}

	FVector start = OwnerCharacter->GetActorLocation();
	FVector target = Target->GetActorLocation();

	FRotator rotator = UKismetMathLibrary::FindLookAtRotation(start, target);
	FRotator current = OwnerCharacter->GetControlRotation();


	// 보간을 통해 자연스럽게 가도록 설정
	rotator = UKismetMathLibrary::RInterpTo(current, rotator, DeltaTime, InterSpeed);

	// 카메라 로테이션을 회전 결과에 대입하기 위함.
	OwnerCharacter->GetController()->SetControlRotation(rotator);

}

void UCTargetComponent::ToggleTarget()
{
	// 타겟이 이미 잡혀있다면 종료 
	if (!!Target)
	{
		EndTargeting();
		return;
	}
	// 없으면 새로 타겟 잡기
	StartTargeting();
}

void UCTargetComponent::ChangeTargetLeft()
{
	ChangeTarget(false);
}

void UCTargetComponent::ChangeTargetRight()
{
	ChangeTarget(true);
}

void UCTargetComponent::StartTargeting()
{
	SetTraceTargets();
	SetTarget();
}

void UCTargetComponent::EndTargeting()
{
	Target = NULL;
	TraceTargets.Empty();

	if (!!Attached)
		Attached->DestroyComponent();
}

void UCTargetComponent::SetTraceTargets()
{
	// 트레이스 타겟 시작과 끝 
	FVector start = OwnerCharacter->GetActorLocation();
	FVector end = FVector(start.X, start.Y, start.Z + 1);


	// 본인 제외 
	TArray<AActor*> ignoreActors;
	ignoreActors.Add(OwnerCharacter);

	// 구체 모양의 추적 디버그 라인 생성
	// Pawn 채널에 맞는 것만 검출
	// 검출된 내역들은 hitResults에 담기
	TArray<FHitResult> hitResults;
	UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), start, end, TraceRadius,
		"Pawn", false, ignoreActors, Debug, hitResults, true, FLinearColor::Green,
		FLinearColor::Red,1.0f);

	// 검출된 hiyResults들은 내부 순환을 돌면서, 클래스가 같으면 넘어가고
	//즉) 본인은 제외(OwnerCharacter)

	for (const FHitResult& result : hitResults)
	{
		if (result.GetActor()->GetClass() == OwnerCharacter->GetClass())
		continue;

		// character 에 검출된 결과의 액터들을 캐스트 시킴 

		ACharacter* character = Cast<ACharacter>(result.GetActor());

		// 추적 라인에 닿은 캐릭터가 상대방일때, TraceTargets에 넣는다.
		// 겹치지 않게 하기 위해서, 한번만 넣으려고 함
		if (!!character)
			TraceTargets.AddUnique(character);
	}


}

void UCTargetComponent::SetTarget()
{
	float angle = -2.0f;
	ACharacter* target = NULL;

	// 캐릭터의 유일한 타겟이 들어오면 (검출된)
	for (ACharacter* character : TraceTargets)
	{
		// 캐릭터가 정면으로 향하고 있는 벡터
		FVector direction = FQuat(OwnerCharacter->GetControlRotation()).GetForwardVector();
		// 검출된 타겟의 위치에서 내 캐릭터의 위치를 빼면, 둘 사이의 거리가 나옴. 방향은 내 캐릭터 위치에서 타겟의 위치로 향하는 벡터
		FVector offset = character->GetActorLocation() - OwnerCharacter->GetActorLocation();
		// 그 벡터를 정규화 시킴 
		offset = offset.GetSafeNormal();
		
		//direction 과 offset을 내적함.
		float temp = direction | offset;


		if (temp < angle)
			continue;

		angle = temp;
		target = character;
	}

	ChangeCursor(target);
}

void UCTargetComponent::ChangeTarget(bool InRight)
{
	TMap<float, ACharacter*> map;
	for (ACharacter* character : TraceTargets)
	{
		if (Target == character)
			continue;

		FVector targetLocation = character->GetActorLocation();
		FVector ownerLocation = OwnerCharacter->GetActorLocation();
		FVector ownerToTarget = targetLocation - ownerLocation;

		FQuat quat = FQuat(OwnerCharacter->GetControlRotation());
		// 현재 회전을 기준으로 전방 벡터 가져옴
		FVector forward = quat.GetForwardVector();
		FVector up = quat.GetUpVector();

		FVector cross = forward ^ ownerToTarget;
		float dot = cross | up;

		map.Add(dot, character);
	}

	float minimum = FLT_MAX;
	ACharacter* target = NULL;

	TArray<float> keys;
	map.GetKeys(keys);
	for (float key : keys)
	{
		if (InRight == true)
		{
			if (key < 0.0f)
				continue;
		}
		else
			if (key > 0.0f)
				continue;

		if (FMath::Abs(key) > minimum)
			continue;

		minimum = FMath::Abs(key);
		target = *map.Find(key);
	}

	ChangeCursor(target);
}

void UCTargetComponent::ChangeCursor(class ACharacter* InTarget)
{
	if (!!InTarget)
	{
		if (!!Attached)
			Attached->DestroyComponent();

		Attached = UGameplayStatics::SpawnEmitterAttached(Particle, InTarget->GetMesh(), "Spine_Target");
		Target = InTarget;

		return;
	}
	EndTargeting();
}

