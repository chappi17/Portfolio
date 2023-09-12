#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CFootIKComponent.generated.h"

typedef struct IK_TraceInfo
{
	float	fOffset;
	FVector pImpactLocation;
};

USTRUCT(Atomic, BlueprintType)
struct FIK_AnimValue
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float		EffectorLocation_Left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float		EffectorLocation_Right;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator	FootRotation_Left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator	FootRotation_Right;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float		HipOffset;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MY_OWN2_API UCFootIKComponent : public UActorComponent
{
	GENERATED_BODY()


private:
	UPROPERTY()
		class ACharacter* Character;
private:
	float CapsuleHeight;
	FIK_AnimValue IKAnimValues;
	bool Active = false;
	float DeltaTime = 0.0f;
public:
	FORCEINLINE FIK_AnimValue GetIKAnimValue() { return IKAnimValues; }
	FORCEINLINE bool GetIKDebugState() { return Debug; }
	//! Default Values

	//! Left foot bone name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values Socket")
		FName IKSocketName_LeftFoot;
	//! Right foot bone name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values Socket")
		FName IKSocketName_RightFoot;

	//! Distance between the floor and the foot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values")
		float IKAdjustOffset;

	//! Max Linetrace distance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values")
		float IKTraceDistance;

	//! Hip(pelvis) move speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values")
		float IKHipsInterpSpeed;

	//! Leg joing move speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values")
		float IKFeetInterpSpeed;

	//! Enable debug mode
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Debug")
		bool Debug;

public:	
		UCFootIKComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTimes, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//! Set foot bone name from skeletalmesh
	void Set_IKSocketName(FString sSocName_LeftFoot, FString sSocName_RightFoot);

	//! Reset IK State
	void IK_ResetVars();

	//! Set IK Active state
	void SetIKActive(bool bActive);

	//! Set IK Debug state
	void SetIKDebug(bool bActive);
private:
	//! IK process update
	void IK_Update(float fDeltaTime);

	//! IK Debug mode update
	void IK_Debug();

	//! process IK Foot,leg Z Offset
	void IK_Update_FootOffset(float fDeltaTime, float fTargetValue, float* fEffectorValue, float fInterpSpeed);

	//! Process IK Foot rotation from ground normal
	void IK_Update_FootRotation(float fDeltaTime, FRotator pTargetValue, FRotator* pFootRotatorValue, float fInterpSpeed);

	//! Process IK characters capsule component Z Offset
	void IK_Update_CapsuleHalfHeight(float fDeltaTime, float fHipsShifts, bool bResetDefault);

	//! Get foot line trace info
	IK_TraceInfo IK_FootTrace(float fTraceDistance, FName sSocket);

	//! Get ground normal
	FRotator NormalToRotator(FVector pVector);

	//! Check owner character is moving
	bool IsMoving();
		
};
