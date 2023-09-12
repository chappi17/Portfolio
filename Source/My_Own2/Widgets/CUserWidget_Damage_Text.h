#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Damage_Text.generated.h"

UCLASS()
class MY_OWN2_API UCUserWidget_Damage_Text : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
		void DamageText(float Damage);
};
