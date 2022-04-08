// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameMode.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "AI/SAICharacter.h"
#include "SAttributeComponent.h"
#include "EngineUtils.h"

ASGameMode::ASGameMode()
{
	SpawnTimerInterval = 2.0f;
}

void ASGameMode::StartPlay()
{
	Super::StartPlay();
	// set timer for bot spawn
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameMode::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}
// when spawn timer ended - get location point from EQS for spawn
void ASGameMode::SpawnBotTimerElapsed()
{
	// check how many bot are already on level
	int32 NumOfAliveBots = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;
		USAttributeComponent* AttribComp = USAttributeComponent::GetAttributes(Bot);
		if (AttribComp && AttribComp->IsAlive())
		{
			NumOfAliveBots++;
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Found %i alive bots"), NumOfAliveBots);

	if (DifficultyCurve)
	{
		MaxNumOfBots = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NumOfAliveBots >= MaxNumOfBots)
	{
		return;
	}

	UEnvQueryInstanceBlueprintWrapper* QueryInstance =  UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameMode::OnQueryCompleted);
	}
	
}
// on completion of query spawn bot on EQS chosen location (if theres is not already max number of bots)
void ASGameMode::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query failed!"));
		return;
	}
	
	// spawn
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (Locations.Num() > 0)
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
	}
}
