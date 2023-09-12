#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CActionData.generated.h"

UCLASS()
class MY_OWN2_API UCActionData : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditAnywhere) // 무엇을 장착 했는가에 대한 클래스 참조 
		TSubclassOf<class ACAttachment> AttachmentClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere) // 무엇을 장착 했는가에 대한 클래스 참조 
		TSubclassOf<class ACEquipment> EquipmentClass;
	UPROPERTY(BlueprintReadOnly, EditAnywhere) // 무엇을 장착 했는가에 대한 클래스 참조 
		FEquipmentData EquipmentData;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FLinearColor EquipmentColor;
	UPROPERTY(BlueprintReadOnly, EditAnywhere) // 평타 받는 클래스 참조
		TSubclassOf<class ACDoAction> DoActionStyle;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FDoActionData> DoActionDatas;

	void BeginPlay(class ACharacter* InOwnerCharacter,class UCAction** OutAction);

private:
	FString GetLableName(class ACharacter* InOwnerCharacter, FString InName);

};
