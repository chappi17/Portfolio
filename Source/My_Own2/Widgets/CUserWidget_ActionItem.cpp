#include "Widgets/CUserWidget_ActionItem.h"
#include "Global.h"
#include "CUserWidget_ActionItemList.h"
#include "CPlayer.h"

void UCUserWidget_ActionItem::Click()
{
	GetActionList()->Clicked(GetName());
}


void UCUserWidget_ActionItem::Hover()
{
	GetActionList()->Hovered(GetName());
}

void UCUserWidget_ActionItem::UnHover()
{
	GetActionList()->UnHovered(GetName());
}

UCUserWidget_ActionItemList* UCUserWidget_ActionItem::GetActionList()
{
	ACPlayer* player = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	return player->GetActionList();
}
