#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CGameStateBase.generated.h"

UCLASS()
class MY_OWN2_API ACGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	ACGameStateBase(); // »ý¼ºÀÚ

	UFUNCTION()
		void SetScore(int32 NewScore);

	UFUNCTION()
		int32 getScore();
private:
	UPROPERTY()
		int32 CurrentScore;
};
