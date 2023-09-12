#include "Components/COptionComponent.h"

UCOptionComponent::UCOptionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCOptionComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


void UCOptionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

