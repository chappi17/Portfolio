#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CTargetComponent.generated.h"
// Ÿ�� ī�޶� ������Ʈ
// Ÿ�� ��� ī�޶� �����ϴ� ���
// ������ �Ӽ��� ���� �ݰ�, �ӵ�, �����, ��ƼŬ �ʿ� 
// �Լ��� Ÿ�� ��� ���Ű, Ÿ�� �¿�� �ٲٴ� �Լ�, Ÿ�� ����~ ��, Ÿ�� �����ϴ� �Լ�
// Ŀ���� Ÿ�� �ٲٴ� �Լ�
// ��� ���� �������� ���� ����� �ݰ� ���;� �ϴϱ� OwnerCharacter
// ��� Ÿ��, Ŀ�� ���õ� ��ƼŬ, �߰��ϴ� Ÿ���� �迭


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MY_OWN2_API UCTargetComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		float TraceRadius = 1000.0f;
	UPROPERTY(EditAnywhere)
		float InterSpeed = 2.5f;
	UPROPERTY(EditAnywhere)
		TEnumAsByte<EDrawDebugTrace::Type> Debug;
	UPROPERTY(EditAnywhere)
		class UParticleSystem* Particle;
public:	
	UCTargetComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void ToggleTarget();
	void ChangeTargetLeft();
	void ChangeTargetRight();

private:
	void StartTargeting();
	void EndTargeting();

	void SetTraceTargets();
	void SetTarget();

	void ChangeTarget(bool InRight);
	void ChangeCursor(class ACharacter* InTarget);

private:
	class ACharacter* OwnerCharacter;
	class ACharacter* Target;
	class UParticleSystemComponent* Attached;

	TArray<class ACharacter*> TraceTargets;
		
};
