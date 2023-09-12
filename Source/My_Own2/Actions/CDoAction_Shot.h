#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "Widgets/CUserWidget_AmmoUI.h"
#include "CDoAction_Shot.generated.h"

UCLASS()
class MY_OWN2_API ACDoAction_Shot : public ACDoAction
{
	GENERATED_BODY()
public:

	FORCEINLINE int GetBullet() { return Bullet; }
	FORCEINLINE int GetMaxGunBullet() { return MaxAmmo_gun; }
	FORCEINLINE int GetMaxShotGunBullet() { return MaxAmmo_Shotgun; }
protected:
	ACDoAction_Shot();
	virtual void BeginPlay() override;

public:
	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

	virtual void Tick(float DeltaTime) override;

	virtual void OnAim() override;
	virtual void OffAim() override;

private:
	UFUNCTION()
		void OnThrowBeginOverlap(FHitResult InHitResult);
	UPROPERTY()
		UCUserWidget_AmmoUI* AmmoWidgetUI;

private:
	class UCAim* Aim;
	int Bullet = 0;
	bool Reloading = false;
	int MaxAmmo_gun = 20;
	int MaxAmmo_Shotgun = 6;

};
