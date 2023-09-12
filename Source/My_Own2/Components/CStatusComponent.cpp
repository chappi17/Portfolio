#include "Components/CStatusComponent.h"
#include "Components/CustomMovementComponent.h"
#include "Global.h"

UCStatusComponent::UCStatusComponent()
{
}


void UCStatusComponent::SetMove()
{
	bCanMove = true;
}

void UCStatusComponent::SetStop()
{
	bCanMove = false;
}

void UCStatusComponent::AddHealth(float InAmount)
{
	Health += InAmount;

	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
}

void UCStatusComponent::SubHealth(float InAmount)
{
	Health -= InAmount;

	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
}

void UCStatusComponent::SetSpeed(ECharacterSpeed InType)
{
    AActor* Owner = GetOwner();
    if (Owner)
    {
        UCustomMovementComponent* Movement = Owner->FindComponentByClass<UCustomMovementComponent>();
        if (Movement)
        {
            Movement->MaxWalkSpeed = Speed[static_cast<int32>(InType)];
        }
    }
}

void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	Stamina = MaxStamina;
}


