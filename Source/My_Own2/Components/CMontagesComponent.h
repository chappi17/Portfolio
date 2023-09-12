#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/CStateComponent.h"
#include "Engine/DataTable.h"
#include "CMontagesComponent.generated.h"

USTRUCT(BlueprintType)
struct FMontageData : public FTableRowBase
{
	GENERATED_BODY()

public: //몽타주 들어왔을때 설정할 값들
	UPROPERTY(EditAnywhere)
		EStateType Type;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere)
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		FName StartSection;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MY_OWN2_API UCMontagesComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, Category = "DataTable")
		UDataTable* DataTable; // DataTable로 값 설정

public:	
	UCMontagesComponent();

protected:
	virtual void BeginPlay() override;

public:
	void PlayRoll();
	void PlayBackstep();
	void PlayHitted();
	void PlayDead();

private:
	void PlayAnimMontage(EStateType InState);

	FMontageData* Datas[(int32) EStateType::Max]; // State에 따른 Montage 값 설정
		
};
