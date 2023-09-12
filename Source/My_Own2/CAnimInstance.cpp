#include "CAnimInstance.h"
#include "Global.h"
#include "GameFramework/Character.h"
//#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CustomMovementComponent.h"
#include "CPlayer.h"

void UCAnimInstance::GetIsClimbing()
{
	bIsClimbing = CustomMovementComponent->IsClimbing();
}

void UCAnimInstance::GetClimbVelocity()
{
	ClimbVelocity = CustomMovementComponent->GetUnrotatedClimbVelocity();
}

void UCAnimInstance::GetIsFalling()
{
	bIsFalling = CustomMovementComponent->IsFalling();
}


void UCAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ClimbingSystemCharacter = Cast<ACPlayer>(TryGetPawnOwner());

	if (ClimbingSystemCharacter)
	{
		CustomMovementComponent = ClimbingSystemCharacter->GetCustomMovementComponent();
	}
}

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	ACharacter* character = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(character);

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(character);
	CheckNull(action);

	action->OnActionTypeChanged.AddDynamic(this, &UCAnimInstance::OnActionTypeChanged);


}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	ACharacter* character = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(character);

	speed = character->GetVelocity().Size2D();
	Direction = CalculateDirection(character->GetVelocity(), character->GetControlRotation());


	Pitch = character->GetBaseAimRotation().Pitch;

	UCFeetComponent* feet = CHelpers::GetComponent<UCFeetComponent>(character);
	if (!!feet)
	{
		FeetData = feet->GetData();
	}


	if (!ClimbingSystemCharacter || !CustomMovementComponent) return;
	GetIsClimbing();
	GetClimbVelocity();
	GetIsFalling();
};

void UCAnimInstance::OnActionTypeChanged(EActionType InPrevType, EActionType InNewType)
{
	ActionType = InNewType;
}

//void UCAnimInstance::Init_IKFoot()
//{
////	! Get IKFoot Component from owner
//	//APawn* pOwner = TryGetPawnOwner();
//	//if (pOwner != nullptr)
//	//{
//	//	UActorComponent* pActorComp = pOwner->GetComponentByClass(UCFootIKComponent::StaticClass());
//	//	if (pActorComp != nullptr)
//	//	{
//	//		IK_Foot = Cast<UCFootIKComponent>(pActorComp);
//	//		if (IK_Foot == nullptr)
//	//		{
//	//			UE_LOG(LogTemp, Warning, TEXT("IKFootRef is nullptr"));
//	//		}
//	//	}
//	//}
//
//
//	APawn* pOwner = TryGetPawnOwner();
//	if (pOwner != nullptr)
//	{
//		CHelpers::GetComponent<UCFootIKComponent>(pOwner);
//		IK_Foot = Cast<UCFootIKComponent>(pOwner);
//	}
//}

