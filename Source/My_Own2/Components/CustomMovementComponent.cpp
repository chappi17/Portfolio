#include "Components/CustomMovementComponent.h"
#include "Global.h"
#include "Components/CapsuleComponent.h"
#include "CPlayer.h"
#include "Components/CStatusComponent.h"

#pragma region ClimbTraces

void UCustomMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	if (CharacterOwner && CharacterOwner->GetMesh())
	{
		OwningPlayerAnimInstance = CharacterOwner->GetMesh()->GetAnimInstance();
	}

//	OwningPlayerAnimInstance = CharacterOwner->GetMesh()->GetAnimInstance();

	if (OwningPlayerAnimInstance)
	{
		OwningPlayerAnimInstance->OnMontageEnded.AddDynamic(this, &UCustomMovementComponent::OnClimbMontageEnded);
		OwningPlayerAnimInstance->OnMontageBlendingOut.AddDynamic(this, &UCustomMovementComponent::OnClimbMontageEnded);
	}
}


void UCustomMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	if (IsClimbing())
	{
		bOrientRotationToMovement = false;
		CharacterOwner->GetCapsuleComponent()->SetCapsuleHalfHeight(48.f);
	}

	if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == ECustomMovementMode::MOVE_Climb)
	{
		bOrientRotationToMovement = true;
		CharacterOwner->GetCapsuleComponent()->SetCapsuleHalfHeight(96.f);

		const FRotator DirtyRotation = UpdatedComponent->GetComponentRotation();
		const FRotator CleanStandRotation = FRotator(0.0f, DirtyRotation.Yaw, 0.0f);
		UpdatedComponent->SetRelativeRotation(CleanStandRotation);
		StopMovementImmediately();
	}

	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
}

void UCustomMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	if (IsClimbing())
	{
		PhysClimb(deltaTime, Iterations);
	}

	Super::PhysCustom(deltaTime, Iterations);
}

float UCustomMovementComponent::GetMaxSpeed() const
{
	if (IsClimbing())
	{
		return MaxClimbSpeed;
	}
	else
	{
		return Super::GetMaxSpeed();
	}

}

float UCustomMovementComponent::GetMaxAcceleration() const
{
	if (IsClimbing())
	{
		return MaxClimbAcceleration;
	}
	else
	{
		return Super::GetMaxAcceleration();
	}
}

FVector UCustomMovementComponent::ConstrainAnimRootMotionVelocity(const FVector& RootMotionVelocity, const FVector& CurrentVelocity) const
{
	const bool bIsPlayingRMMontage =
		IsFalling() && OwningPlayerAnimInstance && OwningPlayerAnimInstance->IsAnyMontagePlaying();

	if (bIsPlayingRMMontage)
	{
		return RootMotionVelocity;
	}
	else
	{
		return Super::ConstrainAnimRootMotionVelocity(RootMotionVelocity, CurrentVelocity);
	}
}

TArray<FHitResult> UCustomMovementComponent::DoCapsuleTraceMultiByObject(const FVector& Start, const FVector& End, bool bShowDebugShape, bool bDrawPersistantShapes)
{
	TArray<FHitResult> OutCapsuleTraceHitResults;

	EDrawDebugTrace::Type DebugTraceType = EDrawDebugTrace::None;

	if (bShowDebugShape)
	{
		DebugTraceType = EDrawDebugTrace::ForOneFrame;

		if (bDrawPersistantShapes)
		{
			DebugTraceType = EDrawDebugTrace::Persistent;
		}
	}
	UKismetSystemLibrary::CapsuleTraceMultiForObjects(
		this,
		Start,
		End,
		ClimbCapsuleTraceRadius,
		ClimbCapsuleTraceHalfHeight,
		ClimbableSurfaceTraceTypes,
		false,
		TArray<AActor*>(),
		DebugTraceType,
		OutCapsuleTraceHitResults,
		false
	);

	return OutCapsuleTraceHitResults;
}

FHitResult UCustomMovementComponent::DoLineTraceSinglebyObject(const FVector& Start, const FVector& End, bool bShowDebugShape, bool bDrawPersistantShapes)
{
	FHitResult OutHit;

	EDrawDebugTrace::Type DebugTraceType = EDrawDebugTrace::None;

	if (bShowDebugShape)
	{
		DebugTraceType = EDrawDebugTrace::ForOneFrame;

		if (bDrawPersistantShapes)
		{
			DebugTraceType = EDrawDebugTrace::Persistent;
		}
	}

	UKismetSystemLibrary::LineTraceSingleForObjects(
		this,
		Start,
		End,
		ClimbableSurfaceTraceTypes,
		false,
		TArray<AActor*>(),
		DebugTraceType,
		OutHit,
		false
	);
	return OutHit;
}

#pragma endregion

bool UCustomMovementComponent::TraceClimbableSurfaces()
{
	const FVector StartOffset = UpdatedComponent->GetForwardVector() * 30.f;
	const FVector Start = UpdatedComponent->GetComponentLocation() + StartOffset;
	const FVector End = Start + UpdatedComponent->GetForwardVector();
	ClimbableSurfaceTracedResults = DoCapsuleTraceMultiByObject(Start,End); // true 하면 디버그 모드 On
	return !(ClimbableSurfaceTracedResults.Num() == 0);
}

FHitResult UCustomMovementComponent::TraceFromEyeHeight(float TraceDistance, float TraceStartOffset)
{
	const FVector ComponentLocation = UpdatedComponent->GetComponentLocation();
	const FVector EyeHeightOffset = UpdatedComponent->GetUpVector() * (CharacterOwner->BaseEyeHeight + TraceStartOffset);
	const FVector Start = ComponentLocation + EyeHeightOffset;
	const FVector End = Start + UpdatedComponent->GetForwardVector() * TraceDistance;
	// 디버그 하는 곳
	return DoLineTraceSinglebyObject(Start, End);

}

bool UCustomMovementComponent::CanStartClimbing()
{
	if (IsFalling())
		return false;

	if (!TraceClimbableSurfaces())
	{
		return false;
	}

	if (!TraceFromEyeHeight(100.f).bBlockingHit)
	{
		return false;
	}
	return true;
}

void UCustomMovementComponent::StartClimbing()
{
	SetMovementMode(MOVE_Custom, ECustomMovementMode::MOVE_Climb);
//	CLog::Print("ChangeMode");
}

void UCustomMovementComponent::StopClimbing()
{
	SetMovementMode(MOVE_Falling);

	
}

void UCustomMovementComponent::PhysClimb(float deltaTime, int32 Iterations)
{
	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}
	/*Process all the climbable surfaces info*/
	TraceClimbableSurfaces();
	ProcessClimableSurfaceInfo();
	/*Check if we should stop climbing*/
	if (CheckShouldStopClimbing() )
	{
		StopClimbing();
	}
	RestorePreAdditiveRootMotionVelocity();
	if (!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{	//Define the max climb speed and acceleration
		CalcVelocity(deltaTime, 0.f, true, MaxBreakClimbDeceleration);
	}
	ApplyRootMotionToVelocity(deltaTime);
	FVector OldLocation = UpdatedComponent->GetComponentLocation();
	const FVector Adjusted = Velocity * deltaTime;
	FHitResult Hit(1.f);
	//Handle climb rotation
	SafeMoveUpdatedComponent(Adjusted, GetClimbRotation(deltaTime), true, Hit);
	if (Hit.Time < 1.f)
	{
		//adjust and try again
		HandleImpact(Hit, deltaTime, Adjusted);
		SlideAlongSurface(Adjusted, (1.f - Hit.Time), Hit.Normal, Hit, true);
	}
	if (!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / deltaTime;
	}

	/*Snap movement to climbable surfaces*/
	SnapMovementToClimableSurfaces(deltaTime);
	if (CheckHasReachedLedge())
	{
		PlayClimbMontage(ClimbToTopMontage);
	}
}

void UCustomMovementComponent::ProcessClimableSurfaceInfo()
{
	CurrentClimableSurfaceLocation = FVector::ZeroVector;
	CurrentClimableSurfaceNormal = FVector::ZeroVector;

//	CLog::Print("ClimbableSurfaceLocaton: " + CurrentClimableSurfaceLocation.ToCompactString());
	if (ClimbableSurfaceTracedResults.Num() == 0)
		return;

	for (const FHitResult& TracedHitResult : ClimbableSurfaceTracedResults)
	{
		CurrentClimableSurfaceLocation += TracedHitResult.ImpactPoint;
		CurrentClimableSurfaceNormal += TracedHitResult.ImpactNormal;
	}
	CurrentClimableSurfaceLocation /= ClimbableSurfaceTracedResults.Num();
	CurrentClimableSurfaceNormal = CurrentClimableSurfaceNormal.GetSafeNormal();

//	CLog::Print("ClimbableSurfaceLocaton: " + CurrentClimableSurfaceLocation.ToCompactString());
//	CLog::Print("ClimbableSurfaceNormal: " + CurrentClimableSurfaceNormal.ToCompactString());
}

FQuat UCustomMovementComponent::GetClimbRotation(float DeltaTime)
{
	const FQuat CurrentQuat = UpdatedComponent->GetComponentQuat();

	if (HasAnimRootMotion() || CurrentRootMotion.HasOverrideVelocity())
	{
		return CurrentQuat;
	}

	const FQuat TargetQuat = FRotationMatrix::MakeFromX(-CurrentClimableSurfaceNormal).ToQuat();

	return FMath::QInterpTo(CurrentQuat, TargetQuat, DeltaTime, 5.f);
}

void UCustomMovementComponent::SnapMovementToClimableSurfaces(float DeltaTime)
{
	const FVector ComponentForward = UpdatedComponent->GetForwardVector();
	const FVector ComponentLocation = UpdatedComponent->GetComponentLocation();

	const FVector ProjectedCharacterToSurface =
		(CurrentClimableSurfaceLocation - ComponentLocation).ProjectOnTo(ComponentForward);

	const FVector SnapVector = -CurrentClimableSurfaceNormal * ProjectedCharacterToSurface.Size();

	UpdatedComponent->MoveComponent(SnapVector * DeltaTime * MaxClimbSpeed,UpdatedComponent->GetComponentQuat(),
		true);
}

bool UCustomMovementComponent::CheckShouldStopClimbing()
{
	if (ClimbableSurfaceTracedResults.Num() == 0)
		return true;

	const float DotResult = FVector::DotProduct(CurrentClimableSurfaceNormal, FVector::UpVector);
	const float DegreeDiff = FMath::RadiansToDegrees( FMath::Acos(DotResult));

	if (DegreeDiff <= 60.0f)
	{// 업벡터 각도가 60 이하면 오르기 모드 멈추기 
		return true;
	}

//	CLog::Print(DegreeDiff);
	return false;
}

bool UCustomMovementComponent::CheckHasReachedLedge()
{
	FHitResult LedgetHitResult = TraceFromEyeHeight(100.f, 50.f);

	if (!LedgetHitResult.bBlockingHit)
	{
		const FVector WalkableSurfaceTraceStart = LedgetHitResult.TraceEnd;

		const FVector DownVector = -UpdatedComponent->GetUpVector();
		const FVector WalkableSurfaceTraceEnd = WalkableSurfaceTraceStart + DownVector * 100.f;

		FHitResult WalkabkeSurfaceHitResult =
			DoLineTraceSinglebyObject(WalkableSurfaceTraceStart, WalkableSurfaceTraceEnd);

		if (WalkabkeSurfaceHitResult.bBlockingHit && GetUnrotatedClimbVelocity().Z > 10.f)
		{
			return true;
		}
	}

	return false;
}

bool UCustomMovementComponent::CanClimbDownLedge()
{
	if (IsFalling())
		return false;

	const FVector ComponentLocation = UpdatedComponent->GetComponentLocation();
	const FVector ComponentForward = UpdatedComponent->GetForwardVector();
	const FVector DownVector = -UpdatedComponent->GetUpVector();

	const FVector WalkableSurfaceTraceStart = ComponentLocation + ComponentForward * ClimbDownWalkableSurfaceTraceOffset;

	const FVector WalkableSurfaceTraceEnd = WalkableSurfaceTraceStart + DownVector * 100.f;

	FHitResult WalkableSurfaceHit =DoLineTraceSinglebyObject(WalkableSurfaceTraceStart, WalkableSurfaceTraceEnd);

	const FVector LedgeTraceStart = WalkableSurfaceHit.TraceStart + ComponentForward * ClimbDownLedgeTraceOffset;
	const FVector LedgeTraceEnd = LedgeTraceStart + DownVector * 300.f;
	// 디버그 켜는 곳 
	FHitResult LedgeTraceHit = DoLineTraceSinglebyObject(LedgeTraceStart, LedgeTraceEnd);


	if (WalkableSurfaceHit.bBlockingHit && !LedgeTraceHit.bBlockingHit)
	{
		return true;
	}
	return false;
}

void UCustomMovementComponent::PlayClimbMontage(UAnimMontage* MontageToPlay)
{
	if (!MontageToPlay)
	{
	//	CLog::Print("Montage is nullptr");
		return;
	}
	if (!OwningPlayerAnimInstance)
	{
	//	CLog::Print("OwningPlayerAnimInstance is nullptr");
		return;
	}
	if (OwningPlayerAnimInstance->IsAnyMontagePlaying())
	{
	//	Clog::print("montage is playing");
		return;
	}

	OwningPlayerAnimInstance->Montage_Play(MontageToPlay);
}

void UCustomMovementComponent::OnClimbMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
//CLog::Print("Climb Montage Ended");
	if (Montage == IdelToClimbMontage || Montage == ClimbDownLedgeMontage)
	{
		StartClimbing();
		if (Montage == ClimbDownLedgeMontage)
		{
			StopMovementImmediately();
		//	CLog::Print("Stop");
		}
	}
	if (Montage == ClimbToTopMontage)
	{
		SetMovementMode(MOVE_Walking);
	}
}

void UCustomMovementComponent::ToggleClimbing(bool bEnableClimb)
{
	if (bEnableClimb)
	{
		if (CanStartClimbing())
		{
			//Enter the climb state
		//	CLog::Print("Can start climbing");
		//	StartClimbing();
			PlayClimbMontage(IdelToClimbMontage);
		}
		else if(CanClimbDownLedge())
		{
			PlayClimbMontage(ClimbDownLedgeMontage);
			//Status->SetStop();
		}
	}
	else
	{
			StopClimbing();
		//Stop climbing
	}
}

bool UCustomMovementComponent::IsClimbing() const
{
	return MovementMode == MOVE_Custom && CustomMovementMode == ECustomMovementMode::MOVE_Climb;
}

FVector UCustomMovementComponent::GetUnrotatedClimbVelocity() const
{
	return UKismetMathLibrary::Quat_UnrotateVector(UpdatedComponent->GetComponentQuat(), Velocity);
}


void UCustomMovementComponent::TickComponent(float DeltaTimes, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTimes, TickType, ThisTickFunction);
	//TraceClimbableSurfaces();
	//TraceFromEyeHeight(100.0f);
	CanClimbDownLedge();
}
