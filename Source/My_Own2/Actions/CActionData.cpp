#include "Actions/CActionData.h"
#include "Global.h"
#include "CAction.h"
#include "CAttachment.h"
#include "CEquipment.h"
#include "CDoAction.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

void UCActionData::BeginPlay(ACharacter* InOwnerCharacter, UCAction** OutAction)
{
	FTransform transform;

	ACAttachment* attachment = NULL;

	if (!!AttachmentClass)
	{
		attachment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACAttachment>(AttachmentClass, transform, InOwnerCharacter);
		attachment->SetActorLabel(GetLableName(InOwnerCharacter, "_Attachment"));
		UGameplayStatics::FinishSpawningActor(attachment, transform);
	}

	ACEquipment* equipment = NULL;
	if (!!EquipmentClass)
	{ // 클래스의 액터 생성 준비 (Deferred)
		equipment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACEquipment>(EquipmentClass, transform, InOwnerCharacter);
		equipment->SetData(EquipmentData);
		equipment->SetColor(EquipmentColor);
		UGameplayStatics::FinishSpawningActor(equipment, transform);
		 //제대로 생성 Finish
		if (!!attachment)
		{
			equipment->OnEquipmentDelegate.AddDynamic(attachment, &ACAttachment::OnEquip);
			equipment->OnUnEquipmentDelegate.AddDynamic(attachment, &ACAttachment::OnUnEquip);
		}
	}
	ACDoAction* doAction = NULL;
	if (!!DoActionStyle)
	{
		doAction = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACDoAction>(DoActionStyle, transform, InOwnerCharacter);
		doAction->AttachToComponent(InOwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		doAction->SetDatas(DoActionDatas);
		UGameplayStatics::FinishSpawningActor(doAction, transform);
		doAction->SetActorLabel(GetLableName(InOwnerCharacter, "doAction"));

		if (!!equipment)
		{
			doAction->SetEquipped(equipment->GetEquipped());
		}


		if (!!attachment)
		{
			attachment->OnAttachmentBeginOverlap.AddDynamic(doAction, &ACDoAction::OnAttachmentBeginOverlap);
			attachment->OnAttachmentEndOverlap.AddDynamic(doAction, &ACDoAction::OnAttachmentEndOverlap);

			attachment->OnAttachmentCollision.AddDynamic(doAction, &ACDoAction::OnAttachmentCollision);
			attachment->OffAttachmentCollision.AddDynamic(doAction, &ACDoAction::OffAttachmentCollision);
		}
	}

	*OutAction = NewObject<UCAction>();
	(*OutAction)->Attachment = attachment;
	(*OutAction)->Equipment = equipment;
	(*OutAction)->DoAction = doAction;
	(*OutAction)->EquipmentColor = EquipmentColor;
}

FString UCActionData::GetLableName(class ACharacter* InOwnerCharacter, FString InName)
{
	FString str;
	str.Append(InOwnerCharacter->GetActorLabel());
	str.Append("_");
	str.Append(InName);
	str.Append("_");
	str.Append(GetName().Replace(L"DA_", L""));

	return str;
}
