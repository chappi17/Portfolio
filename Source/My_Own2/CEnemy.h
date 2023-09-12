#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ICharacter.h"
#include "Components/CStateComponent.h"
#include "CEnemy.generated.h"

UCLASS()
class MY_OWN2_API ACEnemy : public ACharacter , public IICharacter
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Hitted")
		float LaunchAmount = 100.0f;
public:
	ACEnemy();
	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UPROPERTY(BlueprintReadOnly,VisibleDefaultsOnly)
		class UCActionComponent* Action;

	UPROPERTY(VisibleDefaultsOnly)
		class UCMontagesComponent* Montages;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;

	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* NameWidget;
	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* HealthWidget;

	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* DamageTextWidget;

protected:
	virtual void BeginPlay() override;

	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);
	void MoveToLocation(FVector TargetLocation);
	AActor* ChoosePatrolTarget();

protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

public:	
	virtual void Tick(float DeltaTime) override;
	void CheckPatrolTarget();
private:
	void Hitted();
	void Dead();

	UPROPERTY()
	class AAIController* EnemyController;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnyWhere)
		double PatrolRadius = 200.f;

	FTimerHandle PatrolTimer;
	void PatrolTimerFinished();

	UPROPERTY(EditAnyWhere, Category = "AI Navigation")
	float Waitmin = 8.0f;

	UPROPERTY(EditAnyWhere, Category = "AI Navigation")
	float Waitmax = 10.0f;

public:
	virtual void Begin_Dead() override;
	virtual void End_Dead() override;
	virtual void ChangeColor(FLinearColor InColor) override;


	UFUNCTION()
		void RestoreColor();
private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

private:
	class UMaterialInstanceDynamic* BodyMaterial;
	class UMaterialInstanceDynamic* LogoMaterial;

private:
	class AController* DamageInstigator;
	float DamageValue;
};
