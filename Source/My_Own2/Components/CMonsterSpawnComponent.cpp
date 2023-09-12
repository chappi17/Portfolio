#include "Components/CMonsterSpawnComponent.h"
#include "Global.h"
#include "Engine/TriggerBox.h"

UCMonsterSpawnComponent::UCMonsterSpawnComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}



void UCMonsterSpawnComponent::BeginPlay()
{
	Super::BeginPlay();

	if (TriggerBox)
	{
		TriggerBox->OnActorBeginOverlap.AddDynamic(this, &UCMonsterSpawnComponent::OnTriggerEnter);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TriggerBox is not set on %s"), *GetOwner()->GetName());
	}	
	
}

void UCMonsterSpawnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCMonsterSpawnComponent::OnTriggerEnter(AActor* OverlappedActor, AActor* OtherActor)
{
	if (MonsterClass && OtherActor != GetOwner())
	{
		FVector SpawnLocation = TriggerBox->GetActorLocation();
		SpawnLocation.X += 100;
		FTransform NewSpawnTransform(SpawnLocation); // Assigns the modified location to the transform

		AActor* SpawnedMonster = GetWorld()->SpawnActor<AActor>(MonsterClass, NewSpawnTransform.GetLocation(), NewSpawnTransform.GetRotation().Rotator());

		// Additional setup with SpawnedMonster here if needed
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MonsterClass is not set on %s"), *GetOwner()->GetName());
	}
}
