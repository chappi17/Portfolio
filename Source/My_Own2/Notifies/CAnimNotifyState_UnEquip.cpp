#include "Notifies/CAnimNotifyState_UnEquip.h"
#include "Global.h"
#include "Actions/CEquipment.h"
#include "Components/CActionComponent.h"


FString UCAnimNotifyState_UnEquip::GetNotifyName_Implementation() const
{
	return "UnEquip";
}

void UCAnimNotifyState_UnEquip::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	//action->GetCurrent()->GetEquipment()->Begin_UnEquip();
}

void UCAnimNotifyState_UnEquip::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	//action->GetCurrent()->GetEquipment()->End_UnEquip();
}
