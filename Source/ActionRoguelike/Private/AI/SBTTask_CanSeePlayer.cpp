// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_CanSeePlayer.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USBTTask_CanSeePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (MyController)
	{
		ACharacter* MyCharacter = Cast<ACharacter>(MyController->GetPawn());
		if (MyCharacter == nullptr)
		{
			return EBTNodeResult::InProgress;
		}

		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));
		if (TargetActor == nullptr)
		{
			return EBTNodeResult::InProgress;
		}

		bool bLineOfSight = MyController->LineOfSightTo(TargetActor);

		return bLineOfSight ? EBTNodeResult::Failed : EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::InProgress;
}
