#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CFeetComponent.generated.h"

USTRUCT(BlueprintType)
struct FFeetData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector LeftDistance;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector RightDistance;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector PelvisDistance;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FRotator LeftRotation;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FRotator RightRotation;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MY_OWN2_API UCFeetComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "IK")
		TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::None;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "IK")
		FName LeftSocket = "Foot_L";
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "IK")
		FName RightSocket = "Foot_R";
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "IK")
		float TraceDistance = 55;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "IK")
		float InterpSpeed = 17;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "IK")
		float OffsetDistance = 5;
public:
	FORCEINLINE const FFeetData& GetData() { return Data; }

public:	
	UCFeetComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void Trace(FName InSocket, float& OutDistance, FRotator& OutRotation);

	class ACharacter* OwnerCharacter;
	FFeetData Data;
};
