#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ColoredTexture.h"
#include "EnumName.h"
#include "UserProfile.generated.h"

UCLASS(Blueprintable)
class MY_OWN2_API UUserProfile : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Stats)
		float HP;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Stats)
		float Stamina;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Stats)
		float Armor;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Name)
		FName Name;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Unit")
		TSubclassOf<UUserProfile> UClassOfPlayer;
	UPROPERTY(EditAnywhere, meta = (MetaClass = "GameMode"), Category = "Unit")
		FStringClassReference UClassGameMode;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "HUD")
		FColoredTexture Texture;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "HUD")
		TEnumAsByte<Status> status;
//	UPROPERTY(EditAnywhere, meta = (MetaClass = "GameMode"), Category = "Text")
		

};
