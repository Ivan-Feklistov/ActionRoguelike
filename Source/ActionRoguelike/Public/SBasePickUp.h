// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "Components/StaticMeshComponent.h"
#include "SBasePickUp.generated.h"

class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API ASBasePickUp : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
	void Interact_Implementation(APawn* InstigatorPawn);

	

public:	
	// Sets default values for this actor's properties
	ASBasePickUp();



protected:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
