#include "Notifies/CAnimNotify_UnEquip.h"
#include "Global.h"
#include "Actions/CEquipment.h"
#include "Components/CActionComponent.h"

FString UCAnimNotify_UnEquip::GetNotifyName_Implementation() const
{
	return "UnEquip";
}

void UCAnimNotify_UnEquip::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	action->GetCurrent()->GetEquipment()->UnEquip();
}
