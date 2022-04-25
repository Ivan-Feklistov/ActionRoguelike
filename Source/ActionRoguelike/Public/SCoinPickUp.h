// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBasePickUp.h"
#include "SCoinPickUp.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASCoinPickUp : public ASBasePickUp
{
	GENERATED_BODY()
	
public:

	// Sets default values for this actor's properties
	ASCoinPickUp();

	UPROPERTY(EditAnywhere)
	float CoinCost;

	virtual void OnPickUpInteract_Implementation(APawn* InstigatorPawn) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:


};
