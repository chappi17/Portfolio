#include "CDoAction_Melee.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Camera/CameraComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraShakeBase.h"

void ACDoAction_Melee::DoAction()
{
	Super::DoAction();
	CheckFalse(Datas.Num() > 0);

	if (Enable == true)
	{
		Exist = true;
		Enable = false;

		return;
	}

	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	const FDoActionData& data = Datas[0];
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

	data.bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Melee::Begin_DoAction()
{
	Super::Begin_DoAction();

	CheckFalse(Exist);
	Exist = false;


	OwnerCharacter->StopAnimMontage();

	Index++;
	//	CLog::Print("PlusIndex");

	const FDoActionData& data = Datas[Index];
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

	data.bCanMove ? Status->SetMove() : Status->SetStop();

}

void ACDoAction_Melee::End_DoAction()
{
	Super::End_DoAction();

	const FDoActionData& data = Datas[Index];
	OwnerCharacter->StopAnimMontage(data.AnimMontage);

	State->SetIdleMode();
	Status->SetMove();

	Index = 0;
}

void ACDoAction_Melee::OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser,
	class ACharacter* InOtherCharacter)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOtherCharacter);
	CheckNull(InOtherCharacter);

	// 데미지 겹쳐서 중복되는거 방지 
	for (const ACharacter* other : HittedCharacters)
	{
		if (InOtherCharacter == other) // 이미 있으면 ->배열로 돌려놓음  처음 맞았을때만 데미지 들어가고 이후거는 안들어감
			return;
	}
	HittedCharacters.Add(InOtherCharacter);

	// 데미지 보냄
	FDamageEvent e; // 빈구조체 만들어 놓음
	InOtherCharacter->TakeDamage(Datas[Index].Power, e, OwnerCharacter->GetController(), this); // this는 현재 밀리 
//	CLog::Log(InOtherCharacter->GetName());

	// 이펙트 나오게 하기
	UParticleSystem* hitEffect = Datas[Index].Effect;
	if (!!hitEffect)
	{
		FTransform transform = Datas[Index].EffectTransform;
		transform.AddToTranslation(InOtherCharacter->GetActorLocation());

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);
	}
	float hitStop = Datas[Index].HitStop;
	if (FMath::IsNearlyZero(hitStop) == false) // 0 에 가까운 근사치
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1e-3f); // 10의 -3 승
		UKismetSystemLibrary::K2_SetTimer(this, "RestoreDilation", hitStop * 1e-3f, false);
	}

	// 카메라 쉐이크 하기
	TSubclassOf<UCameraShakeBase> shake = Datas[Index].ShakeClass;
	if (shake != NULL)
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(shake);
}

void ACDoAction_Melee::OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser,
	class ACharacter* InOtherCharacter)
{

}

void ACDoAction_Melee::OnAttachmentCollision()
{

}

void ACDoAction_Melee::OffAttachmentCollision()
{
	HittedCharacters.Empty(); // 충돌 끝난시점이라 풀어주면 
}

void ACDoAction_Melee::RestoreDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}
