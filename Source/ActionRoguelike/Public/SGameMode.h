// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "SGameMode.generated.h"

// TODO function for granting credits to player
// TODO query for spawning of coins and potions

class UEnvQuery;
class UCurveFloat;

UCLASS()
class ACTIONROGUELIKE_API ASGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	ASGameMode();

	virtual void StartPlay() override;

	// class of bots to spawn
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;

	// maximum number of bots
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float MaxNumOfBots = 10.f;

	// console command to kill all bots
	UFUNCTION(Exec)
	void KillAll();

	virtual void OnActorKilled(AActor* Victim, AActor* Killer);

	void ChangeScore(AActor* TargetActor, float ScoreDelta);

	UPROPERTY(EditAnywhere)
	float RespawnDelay = 5.0f;

protected:

	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION(BlueprintNativeEvent)
	void RespawnPlayerElapsed(AController* Controller);
	
};
