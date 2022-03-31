// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAttackRange.h"
#include "AIController.h"

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp)
	{
		
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("Target"));
		if (TargetActor)
		{
			bool bWithinRange = false;
			AAIController* MyController = OwnerComp.GetAIOwner();
			if (MyController)
			{
				APawn* AIPawn = MyController->GetPawn();
				if (AIPawn)
				{

					// check distance between ai pawn and target actor
					float Distance = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
					bWithinRange = Distance < 1000.f;
					
					// check if targetActor is within line of sight of ai pawn
					bool bHasLOS;
					if (bWithinRange)
					{
						bHasLOS = MyController->LineOfSightTo(TargetActor);
					}
					BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, (bWithinRange && bHasLOS));
					
					
				}
			}
		}

	}
}
