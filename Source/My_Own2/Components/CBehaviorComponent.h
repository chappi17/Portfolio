#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CBehaviorComponent.generated.h"

UENUM(BlueprintType)
enum class EBehaviorType :uint8
{
	Wait,Approach,Action,Patrol,Hitted,Avoid,
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBehaviorTypeChanged, EBehaviorType, InPrevType, EBehaviorType, InNewType);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MY_OWN2_API UCBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
		FName BehaviorKey = "Behavior";

	UPROPERTY(EditAnywhere)
		FName PlayerKey = "Player";
public:	
	UCBehaviorComponent();

	UFUNCTION(BlueprintPure)
		bool IsWaitMode();
	UFUNCTION(BlueprintPure)
		bool IsApproachMode();
	UFUNCTION(BlueprintPure)
		bool IsActionMode();
	UFUNCTION(BlueprintPure)
		bool IsPatrolMode();
	UFUNCTION(BlueprintPure)
		bool IsHittedMode();
	UFUNCTION(BlueprintPure)
		bool IsAvoidMode();

	FORCEINLINE void SetBlackboard(class UBlackboardComponent* Inblackboard) { Blackboard = Inblackboard; }
protected:
	virtual void BeginPlay() override;


public:
	void SetWaitMode();
	void SetApproachMode();
	void SetActionMode();
	void SetPatrolMode();
	void SetHittedMode();
	void SetAvoidMode();

	class ACPlayer* GetTargetPlayer();

private:
	void ChangeType(EBehaviorType InType);
	EBehaviorType GetType();

public:
	UPROPERTY(BlueprintAssignable)
		FBehaviorTypeChanged OnBehaviorTypeChanged;

private:
	class UBlackboardComponent* Blackboard;
};
