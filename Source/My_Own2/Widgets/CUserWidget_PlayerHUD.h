#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_PlayerHUD.generated.h"

UCLASS()
class MY_OWN2_API UCUserWidget_PlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateHealthProgressBar(float Health, float maxHealth);

	UFUNCTION(BlueprintCallable)
		void UpdateHealth(float Health, float MaxHealth);

protected:
	virtual void NativeConstruct() override;

};
