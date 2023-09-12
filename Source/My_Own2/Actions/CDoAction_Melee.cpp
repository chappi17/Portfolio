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

	// ������ ���ļ� �ߺ��Ǵ°� ���� 
	for (const ACharacter* other : HittedCharacters)
	{
		if (InOtherCharacter == other) // �̹� ������ ->�迭�� ��������  ó�� �¾������� ������ ���� ���İŴ� �ȵ�
			return;
	}
	HittedCharacters.Add(InOtherCharacter);

	// ������ ����
	FDamageEvent e; // ����ü ����� ����
	InOtherCharacter->TakeDamage(Datas[Index].Power, e, OwnerCharacter->GetController(), this); // this�� ���� �и� 
//	CLog::Log(InOtherCharacter->GetName());

	// ����Ʈ ������ �ϱ�
	UParticleSystem* hitEffect = Datas[Index].Effect;
	if (!!hitEffect)
	{
		FTransform transform = Datas[Index].EffectTransform;
		transform.AddToTranslation(InOtherCharacter->GetActorLocation());

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);
	}
	float hitStop = Datas[Index].HitStop;
	if (FMath::IsNearlyZero(hitStop) == false) // 0 �� ����� �ٻ�ġ
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1e-3f); // 10�� -3 ��
		UKismetSystemLibrary::K2_SetTimer(this, "RestoreDilation", hitStop * 1e-3f, false);
	}

	// ī�޶� ����ũ �ϱ�
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
	HittedCharacters.Empty(); // �浹 ���������̶� Ǯ���ָ� 
}

void ACDoAction_Melee::RestoreDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}
