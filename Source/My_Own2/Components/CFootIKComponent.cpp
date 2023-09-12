#include "Components/CFootIKComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"


UCFootIKComponent::UCFootIKComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	IKFeetInterpSpeed = 22.0f;
	IKHipsInterpSpeed = 17.0f;
	IKTraceDistance = 55.0f;
	IKAdjustOffset = 5.0f;

	Debug = false;
}


void UCFootIKComponent::BeginPlay()
{
	Super::BeginPlay();
	DeltaTime = 0.0f;
	Character = Cast<ACharacter>(GetOwner());
	if (Character == nullptr)
		return;

//	CapsuleHeight = Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

//	SetIKActive(true);
	
}

void UCFootIKComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	Character = nullptr;
}

void UCFootIKComponent::TickComponent(float DeltaTimes, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTimes, TickType, ThisTickFunction);
	if (Character == nullptr)	return;
	if(Active == false)		return;

	//IK_Update(DeltaTimes);
//	IK_Debug();
}

void UCFootIKComponent::Set_IKSocketName(FString sSocName_LeftFoot, FString sSocName_RightFoot)
{
	IKSocketName_LeftFoot = FName(*sSocName_LeftFoot);
	IKSocketName_RightFoot = FName(*sSocName_RightFoot);
}

void UCFootIKComponent::IK_ResetVars()
{
	IK_Update_FootOffset(DeltaTime, 0.0f, &IKAnimValues.EffectorLocation_Right, IKFeetInterpSpeed);
	IK_Update_FootOffset(DeltaTime, 0.0f, &IKAnimValues.EffectorLocation_Left, IKFeetInterpSpeed);

	IK_Update_FootRotation(DeltaTime, FRotator::ZeroRotator, &IKAnimValues.FootRotation_Right, IKFeetInterpSpeed);
	IK_Update_FootRotation(DeltaTime, FRotator::ZeroRotator, &IKAnimValues.FootRotation_Left, IKFeetInterpSpeed);

	IK_Update_FootOffset(DeltaTime, 0.0f, &IKAnimValues.HipOffset, IKHipsInterpSpeed);
	IK_Update_CapsuleHalfHeight(DeltaTime, 0.0f, true);
}

void UCFootIKComponent::SetIKActive(bool bActive)
{
	Active = bActive;
}

void UCFootIKComponent::SetIKDebug(bool bActive)
{
//	Debug = Active;
}

void UCFootIKComponent::IK_Update(float fDeltaTime)
{
	DeltaTime = fDeltaTime;

	//! Get Line Trace Info of ground from foot
	IK_TraceInfo pTrace_Left = IK_FootTrace(IKTraceDistance, IKSocketName_LeftFoot);
	IK_TraceInfo pTrace_Right = IK_FootTrace(IKTraceDistance, IKSocketName_RightFoot);

	if (IsMoving() == false)
	{
		//! Update foot rotation from Impact normal
		//IK_Update_FootRotation(fDeltaTime, NormalToRotator(pTrace_Left.pImpactLocation), &IKAnimValues.FootRotation_Left, IKFeetInterpSpeed);
	//	IK_Update_FootRotation(fDeltaTime, NormalToRotator(pTrace_Right.pImpactLocation), &IKAnimValues.FootRotation_Right, IKFeetInterpSpeed);
	}
	else
	{
	//	IK_Update_FootRotation(fDeltaTime, FRotator::ZeroRotator, &IKAnimValues.FootRotation_Left, IKFeetInterpSpeed);
		//IK_Update_FootRotation(fDeltaTime, FRotator::ZeroRotator, &IKAnimValues.FootRotation_Right, IKFeetInterpSpeed);
	}


	//! Update Hip Offset(Z)
	//! Get Hip(Pelvis)Offset from foot offset and set CapsuleComponent height
	float fHipsOffset = UKismetMathLibrary::Min(pTrace_Left.fOffset, pTrace_Right.fOffset);
	if (fHipsOffset < 0.0f == false) fHipsOffset = 0.0f;
	IK_Update_FootOffset(fDeltaTime, fHipsOffset, &IKAnimValues.HipOffset, IKHipsInterpSpeed);
	IK_Update_CapsuleHalfHeight(fDeltaTime, fHipsOffset, false);

	//! Update effector locations of foot
	IK_Update_FootOffset(fDeltaTime, pTrace_Left.fOffset - fHipsOffset, &IKAnimValues.EffectorLocation_Left, IKFeetInterpSpeed);
	IK_Update_FootOffset(fDeltaTime, -1 * (pTrace_Right.fOffset - fHipsOffset), &IKAnimValues.EffectorLocation_Right, IKFeetInterpSpeed);

	//! When foot is close to trace effector location disable IK
	//bool bLeftFootNearly = UKismetMathLibrary::NearlyEqual_FloatFloat(m_stIKAnimValuse.m_fEffectorLocation_Left, pTrace_Left.fOffset - fHipsOffset, 1.0f);
	//bool bRightFootNearly = UKismetMathLibrary::NearlyEqual_FloatFloat(m_stIKAnimValuse.m_fEffectorLocation_Right, pTrace_Right.fOffset - fHipsOffset, 1.0f);
	//if (bLeftFootNearly == true && bRightFootNearly == true) SetIKActive(false);

}

void UCFootIKComponent::IK_Debug()
{
	if (Character != nullptr)
	{
	//	if (Character->GetCapsuleComponent()->bHiddenInGame == Debug)
		{
	//		Character->GetCapsuleComponent()->bHiddenInGame = !Debug;
		}
	}
}

void UCFootIKComponent::IK_Update_FootOffset(float fDeltaTime, float fTargetValue, float* fEffectorValue, float fInterpSpeed)
{
	float InterpValue = UKismetMathLibrary::FInterpTo(*fEffectorValue, fTargetValue, fDeltaTime, fInterpSpeed);
	*fEffectorValue = InterpValue;
}

void UCFootIKComponent::IK_Update_FootRotation(float fDeltaTime, FRotator pTargetValue, FRotator* pFootRotatorValue, float fInterpSpeed)
{
	FRotator pInterpRotator = UKismetMathLibrary::RInterpTo(*pFootRotatorValue, pTargetValue, fDeltaTime, fInterpSpeed);
	*pFootRotatorValue = pInterpRotator;
}

void UCFootIKComponent::IK_Update_CapsuleHalfHeight(float fDeltaTime, float fHipsShifts, bool bResetDefault)
{
	UCapsuleComponent* pCapsule = Character->GetCapsuleComponent();
	if (pCapsule == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("IK : Capsule is NULL"));
		return;
	}

	float fCapsuleHalf = 0.0f;
	if (bResetDefault == true)
	{
		fCapsuleHalf = CapsuleHeight;
	}
	else
	{
		float fHalfAbsSize = UKismetMathLibrary::Abs(fHipsShifts) * 0.5f;
		fCapsuleHalf = CapsuleHeight - fHalfAbsSize;
	}

	float fScaledCapsuleHalfHeight = pCapsule->GetScaledCapsuleHalfHeight();
	float fInterpValue = UKismetMathLibrary::FInterpTo(fScaledCapsuleHalfHeight, fCapsuleHalf, fDeltaTime, IKHipsInterpSpeed);

	pCapsule->SetCapsuleHalfHeight(fInterpValue, true);
}

IK_TraceInfo UCFootIKComponent::IK_FootTrace(float fTraceDistance, FName sSocket)
{
	IK_TraceInfo pTraceInfo;

	//! Set Linetraces startpoint and end point
	FVector pSocketLocation = Character->GetMesh()->GetSocketLocation(sSocket);
	FVector pLine_Start = FVector(pSocketLocation.X, pSocketLocation.Y, Character->GetActorLocation().Z);
	FVector pLine_End = FVector(pSocketLocation.X, pSocketLocation.Y
		, (Character->GetActorLocation().Z - CapsuleHeight) - fTraceDistance);

	//! Process Line Trace
	FHitResult pHitResult;
	TArray<AActor*> pIgnore;
	pIgnore.Add(GetOwner());

	EDrawDebugTrace::Type eDebug = EDrawDebugTrace::None;
	if (Debug == true) eDebug = EDrawDebugTrace::ForOneFrame;

	bool bResult = UKismetSystemLibrary::LineTraceSingle(GetWorld(), pLine_Start, pLine_End,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), true, pIgnore, eDebug, pHitResult, true);

	//! Set ImpactNormal and Offset from HitResult
	pTraceInfo.pImpactLocation = pHitResult.ImpactNormal;
	if (pHitResult.IsValidBlockingHit() == true)
	{
		float fImpactLegth = (pHitResult.ImpactPoint - pHitResult.TraceEnd).Size();
		pTraceInfo.fOffset = IKAdjustOffset + (fImpactLegth - fTraceDistance);
	}
	else
	{
		pTraceInfo.fOffset = 0.0f;
	}

	return pTraceInfo;
}

FRotator UCFootIKComponent::NormalToRotator(FVector pVector)
{
	float fAtan2_1 = UKismetMathLibrary::DegAtan2(pVector.Y, pVector.Z);
	float fAtan2_2 = UKismetMathLibrary::DegAtan2(pVector.X, pVector.Z);
	fAtan2_2 *= -1.0f;
	FRotator pResult = FRotator(fAtan2_2, 0.0f, fAtan2_1);

	return pResult;
}

bool UCFootIKComponent::IsMoving()
{
	float fSpeed = Character->GetVelocity().Size();
	if (fSpeed > 0.0f) return true;
	return false;
}

