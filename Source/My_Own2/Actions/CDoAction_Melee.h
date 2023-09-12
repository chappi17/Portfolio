#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_Melee.generated.h"

UCLASS()
class MY_OWN2_API ACDoAction_Melee : public ACDoAction
{
	GENERATED_BODY()

public:
	FORCEINLINE void EnableCombo() { Enable = true; }
	FORCEINLINE void DisableCombo() { Enable = false; }

	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

	virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser,
		class ACharacter* InOtherCharacter) override;

	virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser,
		class ACharacter* InOtherCharacter) override;

	virtual void OnAttachmentCollision() override;
	virtual void OffAttachmentCollision() override;

private:
	UFUNCTION()
		void RestoreDilation();

private:
	int32 Index;
	bool Exist;
	bool Enable;
	bool Last;

	TArray<class ACharacter*> HittedCharacters;
	
};
