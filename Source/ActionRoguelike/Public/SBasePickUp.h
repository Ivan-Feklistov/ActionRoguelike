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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ResetTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanInteract;

	UFUNCTION(BlueprintNativeEvent)
	void OnPickUpInteract(APawn* InstigatorPawn);

	UFUNCTION()
	void ResetPickUp();

	// Use HIDE and RESET macro for hiding pickup and resetting some time after
	#define RESET GetWorldTimerManager().SetTimer(TimerHandle_ResetTimer, this, &ASBasePickUp::ResetPickUp, ResetTime);
	#define HIDE SetActorHiddenInGame(true); bCanInteract = false;

protected:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void UsePickUp(APawn* InstigatorPawn);

	FTimerHandle TimerHandle_ResetTimer;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
