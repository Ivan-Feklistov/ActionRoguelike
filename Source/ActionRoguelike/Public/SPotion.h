// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SBasePickUp.h"
#include "SPotion.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASPotion : public ASBasePickUp
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPotion();

	UPROPERTY(EditAnywhere)
	float HealthCapacity;

	UPROPERTY(EditAnywhere)
	float PotionCost;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle TimerHandle_ResetTimer;

	UFUNCTION()
	void ResetPotion();

	void UsePotion(APawn* InstigatorPawn);

public:	

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
