#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h" // Enum문은 전방선언 안먹히니까 헤더에 추가 해야함
#include "ICharacter.h"
#include "GenericTeamAgentInterface.h"
#include "Widgets/CUserWidget_PlayerHUD.h"
#include "CPlayer.generated.h"

class UCustomMovementComponent;

UCLASS()
class MY_OWN2_API ACPlayer : public ACharacter, public IICharacter,  public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ACPlayer(const FObjectInitializer& ObjectInitializer);



private:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UCUserWidget_ActionItemList> ActionListClass;

	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleDefaultsOnly)
		class UCOptionComponent* Option;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCFeetComponent* Feet;

	UPROPERTY(VisibleDefaultsOnly)
		class UCMontagesComponent* Montages;

	UPROPERTY(VisibleDefaultsOnly)
		class UCActionComponent* Action;

	UPROPERTY(VisibleDefaultsOnly)
		class UCTargetComponent* Target;


	UPROPERTY(EditDefaultsOnly)
		uint8 TeamId = 0;



public:
	FORCEINLINE class UCUserWidget_ActionItemList* GetActionList() { return ActionList; }


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
		UCustomMovementComponent* CustomMovementComponent;


protected:
	virtual void BeginPlay() override;



public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual FGenericTeamId GetGenericTeamId() const override;

	void SubHealth();
	void GetHitted();

private:
	class UMaterialInstanceDynamic* BodyMaterial;
	class UMaterialInstanceDynamic* LogoMaterial;
	float DashDistance = 6000;

	void OnViewActionList();
	void OffViewActionList();

	void OnMoveForward(float InAxis);
	void OnMoveRight(float InAxis);
	void OnHorizontalLook(float InAxis);
	void OnVerticalLook(float InAxis);
	void CameraZoom(float InAxis);

	void HandleClimbMovementFowardInput(float InAxis);
	void HandleClimbMovementRightInput(float InAxis);

	void Avoid();
	void Dash();

	void MinusHealth();


	void OnWalk();
	void OffWalk();


	UFUNCTION()
	void OnFist();

	UFUNCTION()
	void OnOneHand();

	UFUNCTION()
	void OnTwoHand();

	UFUNCTION()
	void OnGun();

	UFUNCTION()
	void OnShotGun();

	void OnDoAction();
	void OnClimb();

	void OnTarget();
	void OnTargetLeft();
	void OnTargetRight();

	void OnAim();
	void OffAim();

	void Hitted();
	void Dead();

public:	
	void Begin_Roll();
	void Begin_Backstep();

	void End_Roll();
	void End_Backstep();
private:
	class UCUserWidget_ActionItemList* ActionList;

	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* PlayerHealthWidget;

	UPROPERTY(VisibleDefaultsOnly)
		UCUserWidget_PlayerHUD* HealthWidget;

public:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

	FORCEINLINE UCustomMovementComponent* GetCustomMovementComponent() const { return CustomMovementComponent; }


public:
	void ChangeColor(FLinearColor InColor) override;
	class AController* DamageInstigator;
	float DamageValue;
};
