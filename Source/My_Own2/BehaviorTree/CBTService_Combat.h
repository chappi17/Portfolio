#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Combat.generated.h"

UCLASS()
class MY_OWN2_API UCBTService_Combat : public UBTService
{
	GENERATED_BODY()
public:
	UCBTService_Combat();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
