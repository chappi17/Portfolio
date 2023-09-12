#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "GameFramework/Actor.h"
#include "CTriggerBox_Teleport.generated.h"

UCLASS()
class MY_OWN2_API ACTriggerBox_Teleport : public ATriggerBox
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
public:
	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	UPROPERTY(EditAnywhere, Category = "Teleport")
		AActor* TargetPoint;
	UPROPERTY(EditAnywhere, Category = "Particle")
		UParticleSystem* Particle;

};
