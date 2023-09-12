#include "Notifies/CAnimNotifyState_AIAttack.h"
#include "Global.h"
#include "Actions/CActionData.h"
#include "Actions/CAttachment.h"
#include "Components/CActionComponent.h"


FString UCAnimNotifyState_AIAttack::GetNotifyName_Implementation() const
{
	return "AIAttack";
}

void UCAnimNotifyState_AIAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
}

void UCAnimNotifyState_AIAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
}
