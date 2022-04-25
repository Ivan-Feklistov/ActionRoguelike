// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_ProjectileAttack.generated.h"

class UAnimMontage;
class UParticleSystem;

UCLASS()
class ACTIONROGUELIKE_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TSubclassOf<AActor> PRojectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TSubclassOf<AActor> BlackHoleClass;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TSubclassOf<AActor> DashClass;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackRate;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	FName HandSocketName;


};
