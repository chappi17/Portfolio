#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Name.generated.h"

UCLASS()
class MY_OWN2_API UCUserWidget_Name : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
		void SetNameText(const FString& InName);
};
