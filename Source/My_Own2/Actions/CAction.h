#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CAction.generated.h"

USTRUCT(BlueprintType)
struct FEquipmentData // 장착 데이터
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere) // 어떤 몽타주 나올지
		class UAnimMontage* AnimMontage;
	UPROPERTY(EditAnywhere) // 몽타주 플레이 속도
		float PlayRatio = 1.0f;
	UPROPERTY(EditAnywhere) // 특정 시점에서 몽타주 실행할지
		FName StartSection;
	UPROPERTY(EditAnywhere) // 어떤 몽타주 나올지
		class UAnimMontage* UnEquipAnimMontage;
	UPROPERTY(EditAnywhere) // 몽타주 플레이 속도
		float PlayRatio_Unequip = 1.0f;
	UPROPERTY(EditAnywhere) // 특정 시점에서 몽타주 실행할지
		FName StartSection_Unequip;
	UPROPERTY(EditAnywhere) // 실행도중 움직일 수 있는지 
		bool bCanMove = true;
	UPROPERTY(EditAnywhere) // 조작이 가능한지 
		bool bPawnControl = true;

};

USTRUCT(BlueprintType)
struct FDoActionData :public FEquipmentData
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere) // 평타 데미지
		float Power = 5.0f;
	UPROPERTY(EditAnywhere) // 평타 타격감 구현 시간 
		float HitStop;
	UPROPERTY(EditAnywhere) // 평타시 이펙트 
		class UParticleSystem* Effect;
	UPROPERTY(EditAnywhere) // 이펙트 출력 위치 
		FTransform EffectTransform;
	UPROPERTY(EditAnywhere) // 카메라 쉐이크 받는 클래스 참조 
		TSubclassOf<class UCameraShakeBase> ShakeClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACThrow> ThrowClass;

};

UCLASS()
class MY_OWN2_API UCAction : public UObject
{
	GENERATED_BODY()

public:
	friend class UCActionData;

public:
	FORCEINLINE class ACEquipment* GetEquipment() { return Equipment; }
	FORCEINLINE class ACDoAction* GetDoAction() { return DoAction; }
	FORCEINLINE class ACAttachment* GetAttachment() { return Attachment; }

	FORCEINLINE FLinearColor GetEquipmentColor() { return EquipmentColor; }

private:
	class ACEquipment* Equipment;
	class ACAttachment* Attachment;
	class ACDoAction* DoAction;

	FLinearColor EquipmentColor;
	
};
