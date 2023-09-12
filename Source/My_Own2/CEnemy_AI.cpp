#include "Global.h"
#include "CEnemy_AI.h"
#include "CPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/CBehaviorComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Components/CMontagesComponent.h"

ACEnemy_AI::ACEnemy_AI()
{
	PrimaryActorTick.bCanEverTick = true;
}


void ACEnemy_AI::PlayattackMontage()
{

    CurrentComboIndex = 0; // Reset the combo index
    PlayNextMontageInCombo();
    //for (int i = 0; i < MyMontages.Num(); ++i)
    //{
    //    UAnimMontage* CurrentMontage = MyMontages[i];
    //    if (CurrentMontage)
    //    {
    //        PlayAnimMontage(CurrentMontage);
    //    }
    //}

}


void ACEnemy_AI::PlayNextMontageInCombo()
{
    if (CurrentComboIndex < MyMontages.Num())
    {
        UAnimMontage* CurrentMontage = MyMontages[CurrentComboIndex];
        if (CurrentMontage)
        {
            PlayAnimMontage(CurrentMontage);
        }

        // Increment the combo index for the next montage
        CurrentComboIndex++;

        // Set a timer to play the next montage after a delay (e.g., 3 seconds)
        GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &ACEnemy_AI::PlayNextMontageInCombo, 0.3f, false);
    }
    else
    {
        // Clear the timer if we've reached the end of the combo
        GetWorld()->GetTimerManager().ClearTimer(ComboTimerHandle);
    }
}

