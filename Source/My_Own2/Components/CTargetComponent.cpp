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
	// Target�� �׾��ٸ�, true �ٲ㼭 EndTargeting �ϱ� 
	b |= Target->GetDistanceTo(OwnerCharacter) >= TraceRadius;
	// Target�� �Ÿ��� TraceRadius ���� ũ�� �ȴٸ� true �ٲ㼭 EndTargeting �ϱ� 
	// �� Ÿ���� �������� ���ǵ� ����

	if (b == true)
	{
		EndTargeting();

		return;
	}

	FVector start = OwnerCharacter->GetActorLocation();
	FVector target = Target->GetActorLocation();

	FRotator rotator = UKismetMathLibrary::FindLookAtRotation(start, target);
	FRotator current = OwnerCharacter->GetControlRotation();


	// ������ ���� �ڿ������� ������ ����
	rotator = UKismetMathLibrary::RInterpTo(current, rotator, DeltaTime, InterSpeed);

	// ī�޶� �����̼��� ȸ�� ����� �����ϱ� ����.
	OwnerCharacter->GetController()->SetControlRotation(rotator);

}

void UCTargetComponent::ToggleTarget()
{
	// Ÿ���� �̹� �����ִٸ� ���� 
	if (!!Target)
	{
		EndTargeting();
		return;
	}
	// ������ ���� Ÿ�� ���
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
	// Ʈ���̽� Ÿ�� ���۰� �� 
	FVector start = OwnerCharacter->GetActorLocation();
	FVector end = FVector(start.X, start.Y, start.Z + 1);


	// ���� ���� 
	TArray<AActor*> ignoreActors;
	ignoreActors.Add(OwnerCharacter);

	// ��ü ����� ���� ����� ���� ����
	// Pawn ä�ο� �´� �͸� ����
	// ����� �������� hitResults�� ���
	TArray<FHitResult> hitResults;
	UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), start, end, TraceRadius,
		"Pawn", false, ignoreActors, Debug, hitResults, true, FLinearColor::Green,
		FLinearColor::Red,1.0f);

	// ����� hiyResults���� ���� ��ȯ�� ���鼭, Ŭ������ ������ �Ѿ��
	//��) ������ ����(OwnerCharacter)

	for (const FHitResult& result : hitResults)
	{
		if (result.GetActor()->GetClass() == OwnerCharacter->GetClass())
		continue;

		// character �� ����� ����� ���͵��� ĳ��Ʈ ��Ŵ 

		ACharacter* character = Cast<ACharacter>(result.GetActor());

		// ���� ���ο� ���� ĳ���Ͱ� �����϶�, TraceTargets�� �ִ´�.
		// ��ġ�� �ʰ� �ϱ� ���ؼ�, �ѹ��� �������� ��
		if (!!character)
			TraceTargets.AddUnique(character);
	}


}

void UCTargetComponent::SetTarget()
{
	float angle = -2.0f;
	ACharacter* target = NULL;

	// ĳ������ ������ Ÿ���� ������ (�����)
	for (ACharacter* character : TraceTargets)
	{
		// ĳ���Ͱ� �������� ���ϰ� �ִ� ����
		FVector direction = FQuat(OwnerCharacter->GetControlRotation()).GetForwardVector();
		// ����� Ÿ���� ��ġ���� �� ĳ������ ��ġ�� ����, �� ������ �Ÿ��� ����. ������ �� ĳ���� ��ġ���� Ÿ���� ��ġ�� ���ϴ� ����
		FVector offset = character->GetActorLocation() - OwnerCharacter->GetActorLocation();
		// �� ���͸� ����ȭ ��Ŵ 
		offset = offset.GetSafeNormal();
		
		//direction �� offset�� ������.
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
		// ���� ȸ���� �������� ���� ���� ������
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

