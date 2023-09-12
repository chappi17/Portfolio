#include "Notifies/CAnimNotify_EndRoll.h"
#include "Global.h"
#include "CPlayer.h"

FString UCAnimNotify_EndRoll::GetNotifyName_Implementation() const
{
	return "End_Roll";
}

void UCAnimNotify_EndRoll::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	player->End_Roll();

}
