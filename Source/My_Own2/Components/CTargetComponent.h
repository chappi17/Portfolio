#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CTargetComponent.generated.h"
// 타겟 카메라 컴포넌트
// 타겟 잡는 카메라 세팅하는 기능
// 에디터 속성은 추적 반경, 속도, 디버그, 파티클 필요 
// 함수는 타겟 잡는 토글키, 타겟 좌우로 바꾸는 함수, 타겟 시작~ 끝, 타겟 세팅하는 함수
// 커서와 타겟 바꾸는 함수
// 멤버 변수 본인으로 부터 디버그 반경 나와야 하니까 OwnerCharacter
// 상대 타겟, 커서 관련된 파티클, 추격하는 타겟의 배열


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
