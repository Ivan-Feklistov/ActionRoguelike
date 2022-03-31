// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckLowHealth.h"
#include "AI/SAICharacter.h"
#include "AI/SAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckLowHealth::PostInitProperties()
{
	Super::PostInitProperties();

	Interval = 5.f;
	RandomDeviation = 2.f;
}

void USBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr)
	{
		return;
	}
	ASAICharacter* AIChar = Cast<ASAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (AIChar)
	{
		USAttributeComponent* AttribComp = AIChar->GetAttributeComponent();
		if (AttribComp)
		{
			if (AttribComp->Health < AttribComp->MaxHealth)
			{
				BlackboardComp->SetValueAsBool(StartHealKey.SelectedKeyName, true);
			}

		}
	}
}


