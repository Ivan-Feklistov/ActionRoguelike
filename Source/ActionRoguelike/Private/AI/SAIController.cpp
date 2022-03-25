// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree\BlackboardComponent.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);

	APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	if (Player)
	{
		
		GetBlackboardComponent()->SetValueAsObject("Target", Player);
	}
}
