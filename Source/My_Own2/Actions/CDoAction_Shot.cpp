#include "Actions/CDoAction_Shot.h"
#include "Global.h"
#include "CAim.h"
#include "Actions/CThrow.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Widgets/CUserWidget_AmmoUI.h"


ACDoAction_Shot::ACDoAction_Shot()
{
	TSubclassOf<UCUserWidget_AmmoUI> AmmoClass;
	CHelpers::GetClass<UCUserWidget_AmmoUI>(&AmmoClass, "WidgetBlueprint'/Game/WIdget/WB_AmmoUI.WB_AmmoUI_C'");

	if (AmmoClass)
	{
		AmmoWidgetUI = CreateWidget<UCUserWidget_AmmoUI>(GetWorld(), AmmoClass);
		if (AmmoWidgetUI)
		{
			AmmoWidgetUI->AddToViewport();
			AmmoWidgetUI->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}


void ACDoAction_Shot::BeginPlay()
{
	Super::BeginPlay();

	Aim = NewObject<UCAim>();
	Aim->BeginPlay(OwnerCharacter);
}

void ACDoAction_Shot::DoAction()
{
	if (Aim->IsAvaliable())
		CheckFalse(Aim->IsZoom());

	CheckFalse(State->IsIdleMode());
	if (Reloading == true)
	{
		return;
	}

	State->SetActionMode();

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRatio, Datas[0].StartSection);
	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();


	Bullet++;
	CLog::Print(Bullet);

	if (Bullet >= MaxAmmo_gun)
	{
		CLog::Print("Out of Ammo!");
		Reloading = true;

		FTimerHandle Wait;
		float WaitTime = 3.0f;

		GetWorld()->GetTimerManager().SetTimer(Wait, FTimerDelegate::CreateLambda([&]()
			{
				CLog::Print("Reload!");
				Bullet = 0;
				Reloading = false;
			}), WaitTime, false);
	}

	if (AmmoWidgetUI)
	{
		AmmoWidgetUI->Update_Ammo(Bullet, MaxAmmo_gun);
	}

}

void ACDoAction_Shot::Begin_DoAction()
{
	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation("Gun");
	FRotator rotator = OwnerCharacter->GetController()->GetControlRotation();

	FTransform transform = Datas[0].EffectTransform;
	transform.AddToTranslation(location);
	transform.SetRotation(FQuat(rotator));

	FActorSpawnParameters params;
	params.Owner = this;

	FVector direction = rotator.Vector();
	FVector end = location + direction * 1000;
//	DrawDebugLine(GetWorld(), location, end, FColor::Green, true, 3.0f);

		ACThrow* throwObject = OwnerCharacter->GetWorld()->SpawnActorDeferred<ACThrow>(Datas[0].ThrowClass, transform, OwnerCharacter, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		throwObject->OnThrowBeginOverlap.AddDynamic(this, &ACDoAction_Shot::OnThrowBeginOverlap);
		throwObject->SetActorEnableCollision(true); // Enable collision for the throw object
		UGameplayStatics::FinishSpawningActor(throwObject, transform);



	//FVector location = OwnerCharacter->GetMesh()->GetSocketLocation("Gun");
	//FRotator rotator = OwnerCharacter->GetController()->GetControlRotation();

	//FTransform transform = Datas[0].EffectTransform;
	//transform.AddToTranslation(location);
	//transform.SetRotation(FQuat(rotator));

	//FActorSpawnParameters params;
	//params.Owner = this;

	//FVector direction = rotator.Vector();

	//float lineLength = 1000.0f;

	//FVector end = location + direction * 1000;

	//DrawDebugLine(GetWorld(), location, end, FColor::Green, true, 3.0f);


	//ACThrow* throwObject = OwnerCharacter->GetWorld()->SpawnActorDeferred<ACThrow>(Datas[0].ThrowClass, transform, OwnerCharacter, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	////throwObject->OnThrowBeginOverlap.AddDynamic(this, &ACDoAction_Shot::OnThrowBeginOverlap);
	//throwObject->SetActorEnableCollision(true); // Enable collision for the throw object
	//UGameplayStatics::FinishSpawningActor(throwObject, transform);

	//if (!throwObject)
	//{
	//	CLog::Print("No Bullet");
	//}
	//else
	//	CLog::Print("Yes Bullet");

}

void ACDoAction_Shot::End_DoAction()
{
	State->SetIdleMode();
	Status->SetMove();
}

void ACDoAction_Shot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Aim->Tick(DeltaTime);

}

void ACDoAction_Shot::OnAim()
{
	Aim->OnZoom();
}

void ACDoAction_Shot::OffAim()
{
	Aim->OffZoom();
}

void ACDoAction_Shot::OnThrowBeginOverlap(FHitResult InHitResult)
{
	FDamageEvent e;
	InHitResult.GetActor()->TakeDamage(Datas[0].Power, e, OwnerCharacter->GetController(), this);
}
