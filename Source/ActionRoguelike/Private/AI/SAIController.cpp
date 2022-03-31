// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree\BlackboardComponent.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();
	if ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nullptr! Please assign your Behavior Tree to AI Controller!"))
	{
		RunBehaviorTree(BehaviorTree);
	}
	

// 	APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
// 	if (Player)
// 	{
// 		
// 		GetBlackboardComponent()->SetValueAsObject("Target", Player);
// 	}
}
