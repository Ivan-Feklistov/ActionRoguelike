// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameMode.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "AI/SAICharacter.h"
#include "SAttributeComponent.h"
#include "EngineUtils.h"
#include "SCharacter.h"



static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("s.SpawnBots"), true, TEXT("Enable spawn of bots"), ECVF_Cheat);

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

// iterate over all Ai bots and kill them
void ASGameMode::KillAll()
{
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;
		USAttributeComponent* AttribComp = USAttributeComponent::GetAttributes(Bot);
		if (AttribComp && AttribComp->IsAlive())
		{
			AttribComp->Kill(this); // @fixme: maybe change Instigator to player for kill credit?
		}
	}
}

// when spawn timer ended - get location point from EQS for spawn
void ASGameMode::SpawnBotTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn of bots disabled by CVar: s.SpawnBots"));
		return;
	}

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

void ASGameMode::OnActorKilled(AActor* Victim, AActor* Killer)
{
	// if player was killed then respawn after some time
	ASCharacter* Player = Cast<ASCharacter>(Victim);
	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}

	// reward player for kill of bot
	Player = Cast<ASCharacter>(Killer);
	if (Player)
	{
		ASAICharacter* Bot = Cast<ASAICharacter>(Victim);
		if (Bot)
		{
			ChangeScore(Killer, Bot->ScoreCost);
		}
		else
		{
			ChangeScore(Killer, 5);
		}
		
	}
	UE_LOG(LogTemp, Log, TEXT("Actor Killed: Victim - %s, Killer - %s"), *GetNameSafe(Victim), *GetNameSafe(Killer));
}

void ASGameMode::ChangeScore(AActor* TargetActor, float ScoreDelta)
{
	USAttributeComponent* AttribComp = USAttributeComponent::GetAttributes(TargetActor);
	if (AttribComp)
	{
		AttribComp->ChangePlayerScore(ScoreDelta);
	}
	
}

void ASGameMode::RespawnPlayerElapsed_Implementation(AController* Controller)
{
	Controller->UnPossess();

	RestartPlayer(Controller);
}


