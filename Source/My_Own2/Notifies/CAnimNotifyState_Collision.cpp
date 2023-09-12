#include "Notifies/CAnimNotifyState_Collision.h"
#include "Global.h"
#include "Actions/CActionData.h"
#include "Actions/CAttachment.h"
#include "Components/CActionComponent.h"

FString UCAnimNotifyState_Collision::GetNotifyName_Implementation() const
{
	return "Collision";
}

void UCAnimNotifyState_Collision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (!MeshComp)
	{
		CLog::Print("MeshComp is null!");
		return;
	}

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		CLog::Print("Owner is null!");
		return;
	}

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(Owner);
	if (!action)
	{
		CLog::Print("UCActionComponent not found!");
		return;
	}

	if (!action->GetCurrent())
	{
		CLog::Print("GetCurrent() returned null!");
		return;
	}

	ACAttachment* attachment = action->GetCurrent()->GetAttachment();
	if (!attachment)
	{
		CLog::Print("GetAttachment() returned null!");
		return;
	}

	// 이제 여기서 안전하게 attachment->OnCollision()을 호출할 수 있습니다.
	attachment->OnCollision();

}

void UCAnimNotifyState_Collision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	ACAttachment* attachment = action->GetCurrent()->GetAttachment();
	CheckNull(attachment);

	attachment->OffCollision();
}
