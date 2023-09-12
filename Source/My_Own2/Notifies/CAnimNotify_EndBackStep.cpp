#include "Notifies/CAnimNotify_EndBackStep.h"
#include "Global.h"
#include "CPlayer.h"

FString UCAnimNotify_EndBackStep::GetNotifyName_Implementation() const
{
	return "End_BackStep";
}

void UCAnimNotify_EndBackStep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	player->End_Backstep();

}
