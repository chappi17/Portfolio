#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CActionData.generated.h"

UCLASS()
class MY_OWN2_API UCActionData : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditAnywhere) // ������ ���� �ߴ°��� ���� Ŭ���� ���� 
		TSubclassOf<class ACAttachment> AttachmentClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere) // ������ ���� �ߴ°��� ���� Ŭ���� ���� 
		TSubclassOf<class ACEquipment> EquipmentClass;
	UPROPERTY(BlueprintReadOnly, EditAnywhere) // ������ ���� �ߴ°��� ���� Ŭ���� ���� 
		FEquipmentData EquipmentData;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FLinearColor EquipmentColor;
	UPROPERTY(BlueprintReadOnly, EditAnywhere) // ��Ÿ �޴� Ŭ���� ����
		TSubclassOf<class ACDoAction> DoActionStyle;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FDoActionData> DoActionDatas;

	void BeginPlay(class ACharacter* InOwnerCharacter,class UCAction** OutAction);

private:
	FString GetLableName(class ACharacter* InOwnerCharacter, FString InName);

};
