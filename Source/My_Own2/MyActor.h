#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

UCLASS()
class MY_OWN2_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AMyActor();
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Properties")
		FString Name;

//	UFUNCTION(BlueprintCallable, Category = "Properties")
	//	FString ToString();
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
