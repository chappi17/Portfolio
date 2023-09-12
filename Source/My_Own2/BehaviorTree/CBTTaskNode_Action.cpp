#include "BehaviorTree/CBTTaskNode_Action.h"
#include "Global.h"
#include "CAIController.h"
#include "CEnemy_AI.h"
#include "Components/CActionComponent.h"
#include "Components/CStateComponent.h"

UCBTTaskNode_Action::UCBTTaskNode_Action()
{
	bNotifyTick = true;
	NodeName = "Action";
}

EBTNodeResult::Type UCBTTaskNode_Action::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(ai);
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);

	TotalTime = 0.0f;
	ai->PlayattackMontage();
	state->IsIdleMode();

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Action::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);

	TotalTime += DeltaSeconds;

	if (state->IsIdleMode() && TotalTime > Delay)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
