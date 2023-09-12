#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"

UENUM(BlueprintType)
enum class ECharacterSpeed : uint8
{
	Walk, Run, Sprint, Max,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MY_OWN2_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Health")
		float MaxHealth = 100;
	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
		float MaxStamina = 100;
	UPROPERTY(EditDefaultsOnly, Category = "Speed")
		float Speed[(int32)ECharacterSpeed::Max] = { 200, 400, 600 };
public:
	UPROPERTY(BlueprintReadOnly, Category = "Health")
		float playerHealth;
public:	
	UCStatusComponent();


	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE float GetHealth() { return Health; }
	FORCEINLINE float GetMaxStamina() { return MaxStamina; }
	FORCEINLINE float GetStamina() { return Stamina; }

	FORCEINLINE float GetWalkSpeed() { return Speed[(int32)ECharacterSpeed::Walk]; }
	FORCEINLINE float GetRunkSpeed() { return Speed[(int32)ECharacterSpeed::Run]; }
	FORCEINLINE float GetSprintSpeed() { return Speed[(int32)ECharacterSpeed::Sprint]; }

	FORCEINLINE bool CanMove() { return bCanMove; }

	void SetMove();
	void SetStop();

	void AddHealth(float InAmount);
	void SubHealth(float InAmount);

	void SetSpeed(ECharacterSpeed InType);
protected:
	virtual void BeginPlay() override;

private:
	float Health;
	float Stamina;
	bool bCanMove = true;
		
};
