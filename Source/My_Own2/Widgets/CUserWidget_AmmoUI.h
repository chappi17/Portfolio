#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_AmmoUI.generated.h"

UCLASS()
class MY_OWN2_API UCUserWidget_AmmoUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
		void Update_Ammo(int Ammo, int MaxAmmo);
};
