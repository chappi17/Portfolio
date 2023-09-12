#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_ActionItem.generated.h"

UCLASS()
class MY_OWN2_API UCUserWidget_ActionItem : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
		void Click();
	UFUNCTION(BlueprintCallable)
		void Hover();
	UFUNCTION(BlueprintCallable)
		void UnHover();

private:
	class UCUserWidget_ActionItemList* GetActionList();
	
};
