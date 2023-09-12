#pragma once

#include "UObject/ObjectMacros.h"
#include "ColoredTexture.generated.h"

USTRUCT(Blueprintable)
struct MY_OWN2_API FColoredTexture
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "HUD")
		UTexture* Texture;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "HUD")
		FLinearColor Color;
};

