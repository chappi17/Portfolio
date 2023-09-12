#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController.generated.h"

UCLASS()
class MY_OWN2_API ACAIController : public AAIController
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
		float MeleeActionRange = 150;
	UPROPERTY(EditAnywhere)
		bool bDrawDebug =true;
	UPROPERTY(EditAnywhere)
		float AdjustCircleHeight =50;

	UPROPERTY(VisibleDefaultsOnly)
		class UAIPerceptionComponent* Perception;
	UPROPERTY(VisibleDefaultsOnly)
		class UCBehaviorComponent* Behavior;

public:
	FORCEINLINE float GetMeleeActionRange() { return MeleeActionRange; }

	ACAIController();
	float GetSightRadius();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
private:
	UFUNCTION()
		void OnPerceptionUpdated(const TArray<AActor*>& UpdateActors);

private:
	class ACEnemy_AI* OwnerEnemy;
	class UAISenseConfig_Sight* Sight;
};
