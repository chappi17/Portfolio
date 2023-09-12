#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/CActionComponent.h"
#include "Components/CFeetComponent.h"
#include "CAnimInstance.generated.h"

class ACPlayer;
class UCustomMovementComponent;

UCLASS()
class MY_OWN2_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float speed;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Direction;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		EActionType ActionType;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool InAir;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Pitch;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		bool bEquipped;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool Aiming;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FFeetData FeetData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Reference, meta = (AllowPrivateAccess = "true"))
		bool bIsClimbing;
	void GetIsClimbing();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Reference, meta = (AllowPrivateAccess = "true"))
		FVector ClimbVelocity;
	void GetClimbVelocity();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Reference, meta = (AllowPrivateAccess = "true"))
		bool bIsFalling;
	void GetIsFalling();


	UPROPERTY()
		ACPlayer* ClimbingSystemCharacter;

	UPROPERTY()
		UCustomMovementComponent* CustomMovementComponent;

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UFUNCTION()
		void OnActionTypeChanged(EActionType InprevType, EActionType InNewType);


	UPROPERTY()
		class UCFootIKComponent* IK_Foot;


	UPROPERTY()
		ACPlayer* CPlayer;
};
