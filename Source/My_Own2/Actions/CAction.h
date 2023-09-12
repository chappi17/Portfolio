#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CAction.generated.h"

USTRUCT(BlueprintType)
struct FEquipmentData // ���� ������
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere) // � ��Ÿ�� ������
		class UAnimMontage* AnimMontage;
	UPROPERTY(EditAnywhere) // ��Ÿ�� �÷��� �ӵ�
		float PlayRatio = 1.0f;
	UPROPERTY(EditAnywhere) // Ư�� �������� ��Ÿ�� ��������
		FName StartSection;
	UPROPERTY(EditAnywhere) // � ��Ÿ�� ������
		class UAnimMontage* UnEquipAnimMontage;
	UPROPERTY(EditAnywhere) // ��Ÿ�� �÷��� �ӵ�
		float PlayRatio_Unequip = 1.0f;
	UPROPERTY(EditAnywhere) // Ư�� �������� ��Ÿ�� ��������
		FName StartSection_Unequip;
	UPROPERTY(EditAnywhere) // ���൵�� ������ �� �ִ��� 
		bool bCanMove = true;
	UPROPERTY(EditAnywhere) // ������ �������� 
		bool bPawnControl = true;

};

USTRUCT(BlueprintType)
struct FDoActionData :public FEquipmentData
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere) // ��Ÿ ������
		float Power = 5.0f;
	UPROPERTY(EditAnywhere) // ��Ÿ Ÿ�ݰ� ���� �ð� 
		float HitStop;
	UPROPERTY(EditAnywhere) // ��Ÿ�� ����Ʈ 
		class UParticleSystem* Effect;
	UPROPERTY(EditAnywhere) // ����Ʈ ��� ��ġ 
		FTransform EffectTransform;
	UPROPERTY(EditAnywhere) // ī�޶� ����ũ �޴� Ŭ���� ���� 
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
