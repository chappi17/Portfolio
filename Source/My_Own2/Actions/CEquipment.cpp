#include "CEquipment.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "CPlayer.h"
#include "Components/ShapeComponent.h"


ACEquipment::ACEquipment()
{

}

void ACEquipment::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);

	Super::BeginPlay();
	
}


void ACEquipment::Equip_Implementation()
{
	State->SetEquipMode();

	if (Data.AnimMontage != NULL)
	{
		OwnerCharacter->PlayAnimMontage(Data.AnimMontage, Data.PlayRatio, Data.StartSection); // 데이터에 있는 몽타주 실행
	}

	if (Data.bCanMove == false)
	{
		Status->SetStop();
	}

	else
		End_Equip();

	if (Data.bPawnControl == true)
	{
		OwnerCharacter->bUseControllerRotationYaw = true; // 장착한 순간 전방 고정 
		OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	IICharacter* character = Cast<IICharacter>(OwnerCharacter);
	CheckNull(character);
	character->ChangeColor(Color);
}

void ACEquipment::Begin_Equip_Implementation()
{
	if (OnEquipmentDelegate.IsBound())
	{
		OnEquipmentDelegate.Broadcast();
	}
}

void ACEquipment::End_Equip_Implementation()
{
	bEquipped = true;
	Status->SetMove();
	State->SetIdleMode();
//	CLog::Print("IdleMode");
}


void ACEquipment::UnEquip_Implementation()
{

	End_Equip();
	if (Data.UnEquipAnimMontage != NULL)
	{
		OwnerCharacter->PlayAnimMontage(Data.UnEquipAnimMontage, Data.PlayRatio_Unequip, Data.StartSection_Unequip); // 데이터에 있는 몽타주 실행
	}

	if (Data.bCanMove == false)
	{
		Status->SetStop();
	}
	if (OnUnEquipmentDelegate.IsBound())
		OnUnEquipmentDelegate.Broadcast();
	bEquipped = false;
	Status->SetMove();
	State->SetIdleMode();
	//bEquipped = false;
	//Status->SetMove();
	//State->SetIdleMode();

	//if (OnUnEquipmentDelegate.IsBound())
	//	OnUnEquipmentDelegate.Broadcast();

	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;

}
