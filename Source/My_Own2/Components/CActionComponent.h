#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CActionComponent.generated.h"

UENUM(BlueprintType)
enum class EActionType : uint8
{
	Unarmed,
	Fist,
	OneHand,
	TwoHand,
	Gun,
	ShotGun,
	AI,
	Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActionTypeChanged, EActionType, InPrevType, EActionType, InNewType);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MY_OWN2_API UCActionComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
		class UCActionData* DataAssets[(int32)EActionType::Max];
public:	
	UFUNCTION(BlueprintPure)
		FORCEINLINE class UCAction* GetCurrent() { return Datas[(int32)Type]; }
protected:
	virtual void BeginPlay() override;
public:
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsUnarmedMode() { return Type == EActionType::Unarmed; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsFistMode() { return Type == EActionType::Fist; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsOneHandMode() { return Type == EActionType::OneHand; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsTwohandMode() { return Type == EActionType::TwoHand; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsGunMode() { return Type == EActionType::Gun; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsShotGunMode() { return Type == EActionType::ShotGun; } 	
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsAIMode() { return Type == EActionType::AI; }

public:
	UCActionComponent();

	UFUNCTION(BlueprintCallable)
	void SetUnarmedMode();

	UFUNCTION(BlueprintCallable)
	void SetTwoHandMode();

	void SetFistMode();
	void SetOneHandMode();
	void SetGunMode();
	void SetShotGunMode();
	void SetAIMode();

	void OffAllCollision();

	void DoAction();
	void DoAim();
	void UndoAim();

private:
	void SetAimMode(bool InAim);

public:
	UPROPERTY(BlueprintAssignable)
		FActionTypeChanged OnActionTypeChanged;

private:
	void SetMode(EActionType InType);
	void ChangeType(EActionType InNewType);
private:	
	EActionType Type;
	class UCAction* Datas[(int32)EActionType::Max];		
};
