#include "Components/CActionComponent.h"
#include "Global.h"
#include "Actions/CAction.h"
#include "Actions/CDoAction.h"
#include "Actions/CEquipment.h"
#include "Actions/CActionData.h"
#include "Actions/CAttachment.h"
#include "GameFramework/Character.h"

UCActionComponent::UCActionComponent()
{
}

void UCActionComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* character = Cast<ACharacter>(GetOwner());
	for (int32 i = 0; i < (int32)EActionType::Max; i++)
	{
		if (!!DataAssets[i])
			DataAssets[i]->BeginPlay(character, &Datas[i]);
	}
	
}

void UCActionComponent::SetUnarmedMode()
{
	if (!!Datas[(int32)Type])
	{
		ACEquipment* equipment = Datas[(int32)Type]->GetEquipment();
		CheckNull(equipment);

		equipment->UnEquip();
	}

	//ACEquipment* equipment = Datas[(int32)EActionType::Unarmed]->GetEquipment();
	//CheckNull(equipment);

	//equipment->Equip();

	ChangeType(EActionType::Unarmed);
}

void UCActionComponent::SetFistMode()
{
	SetMode(EActionType::Fist);
}

void UCActionComponent::SetOneHandMode()
{
	SetMode(EActionType::OneHand);
}

void UCActionComponent::SetTwoHandMode()
{
	SetMode(EActionType::TwoHand);
}

void UCActionComponent::SetGunMode()
{
	SetMode(EActionType::Gun);
}

void UCActionComponent::SetShotGunMode()
{
	SetMode(EActionType::ShotGun);
}

void UCActionComponent::SetAIMode()
{
	SetMode(EActionType::AI);
}

void UCActionComponent::OffAllCollision()
{
	for (UCAction* data : Datas)
	{
		if (!!data == false)
			continue;

		if (!!data->GetAttachment() == false)
			continue;

		data->GetAttachment()->OffCollision();
	}
}

void UCActionComponent::DoAction()
{
	CheckTrue(IsUnarmedMode());

	if (!!Datas[(int32)Type])
	{
		ACDoAction* action = Datas[(int32)Type]->GetDoAction();
	//	CLog::Print("DoAction_Component");
		if (!!action)
		{
			action->DoAction();
	//		CLog::Print("DoAction_Component2");
		}
	}
}

void UCActionComponent::DoAim()
{
	SetAimMode(true);
}

void UCActionComponent::UndoAim()
{
	SetAimMode(false);
}

void UCActionComponent::SetAimMode(bool InAim)
{
	CheckTrue(IsUnarmedMode());

	if (!!Datas[(int32)Type])
	{
		ACDoAction* action = Datas[(int32)Type]->GetDoAction();

		if (!!action)
			InAim ? action->OnAim() : action->OffAim();
	}
}

void UCActionComponent::SetMode(EActionType InType)
{
	if (Type == InType) //같은 무기 해제
	{
		SetUnarmedMode();

		return;
	}
	else if (IsUnarmedMode() == false) //다른 무기 장착
	{
		ACEquipment* equipment = Datas[(int32)Type]->GetEquipment();
		CheckNull(equipment);

		equipment->UnEquip();
	}

	if (!!Datas[(int32)InType])
	{
		ACEquipment* equipment = Datas[(int32)InType]->GetEquipment();
		CheckNull(equipment);
		equipment->Equip();
	}

	ChangeType(InType);
}

void UCActionComponent::ChangeType(EActionType InNewType)
{
	EActionType preveType = InNewType;
	Type = InNewType;

	if (OnActionTypeChanged.IsBound())
		OnActionTypeChanged.Broadcast(preveType, InNewType);
}



