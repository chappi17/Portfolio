#include "Widgets/CUserWidget_ActionItemList.h"
#include "Global.h"
#include "CUserWidget_Actionitem.h"
#include "Components/GridPanel.h"
#include "Components/Border.h"

void UCUserWidget_ActionItemList::NativeConstruct()
{
	UGridPanel* gridPanel = Cast<UGridPanel>(GetWidgetFromName("Grid"));
	TArray<UWidget*> widgets = gridPanel->GetAllChildren();

	for (UWidget* widget : widgets)
	{
        FUserWidget_Action_Data data;
		data.Item = Cast<UCUserWidget_ActionItem>(widget);

		Datas.Add(data);
	}
	Super::NativeConstruct();
}

void UCUserWidget_ActionItemList::Clicked(FString InName)
{
    for (const FUserWidget_Action_Data& data : Datas)
    {
        if (data.Item->GetName() == InName)
        {
            if (data.OnUserWidget_ActionItem_Clicked.IsBound())
                data.OnUserWidget_ActionItem_Clicked.Broadcast();

            SetVisibility(ESlateVisibility::Hidden);

            UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = false;
            UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeGameOnly());
            UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);

            break;
        }
    }

}


void UCUserWidget_ActionItemList::Hovered(FString InName)
{
    for (const FUserWidget_Action_Data& data : Datas)
    {
        UBorder* border = Cast<UBorder>(data.Item->GetWidgetFromName("BG_Border"));

        if (data.Item->GetName() == InName)
        {
            border->SetBrushColor(FLinearColor::Red);

            break;
        }
    }
}

void UCUserWidget_ActionItemList::UnHovered(FString InName)
{
    for (const FUserWidget_Action_Data& data : Datas)
    {
        UBorder* border = Cast<UBorder>(data.Item->GetWidgetFromName("BG_Border"));

        if (data.Item->GetName() == InName)
        {
            border->SetBrushColor(FLinearColor::White);

            break;
        }
    }
}
