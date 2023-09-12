#pragma once

#include "CoreMinimal.h"
#include "CEnemy.h"
#include "CEnemy_AI.generated.h"

UCLASS()
class MY_OWN2_API ACEnemy_AI : public ACEnemy
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		uint8 TeamID = 1;
public:
	FORCEINLINE class UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }
	FORCEINLINE uint8 GetTeamID() { return TeamID; }

	UPROPERTY(EditAnywhere, Category = "Animation")
		TArray<UAnimMontage*> MyMontages;


	void PlayattackMontage();


private:
	void Attack();

public:
	ACEnemy_AI();

private:
	int32 CurrentComboIndex = 0;
	FTimerHandle ComboTimerHandle;

public:
	UFUNCTION()
		void PlayNextMontageInCombo();
};
