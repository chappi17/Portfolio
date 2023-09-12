#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CHUD.generated.h"

UCLASS()
class MY_OWN2_API ACHUD : public AHUD
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly)
		class UTexture2D* Texture;

public:
	FORCEINLINE void SetDrawMode() { bDraw = true; }
	FORCEINLINE void SetUnDrawMode() { bDraw = false; }

public:
	ACHUD();

	virtual void DrawHUD() override;

private:
	bool bDraw;
};
