#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CEnemy_AI.h"
#include "CMonsterSpawnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MY_OWN2_API UCMonsterSpawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCMonsterSpawnComponent();

	// The class of monster to spawn
	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class ACEnemy_AI> MonsterClass;

	// Spawn transform for the monster
	UPROPERTY(EditAnywhere, Category = "Spawning")
		FTransform SpawnTransform;

	// Reference to the trigger box that will activate the spawn
	UPROPERTY(EditAnywhere, Category = "Trigger")
		class ATriggerBox* TriggerBox;

	UFUNCTION()
		void OnTriggerEnter(AActor* OverlappedActor, AActor* OtherActor);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
