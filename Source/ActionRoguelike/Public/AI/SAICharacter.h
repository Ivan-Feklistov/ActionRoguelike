// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAttributeComponent.h"
#include "SAICharacter.generated.h"


class UPawnSensingComponent;
class USAttributeComponent;
class USWorldUserWidget;
class USActionComponent;

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();


	USAttributeComponent* GetAttributeComponent();

	UPROPERTY(BlueprintReadWrite)
	FVector ForceDirection;

	// number of points rewarded for kill of this bot
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScoreCost;

protected:

	UPROPERTY(EditAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USActionComponent* ActionComp;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDebugPlayerSpotted;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	void SetNewTargetActor(AActor* InstigatorActor);

	UFUNCTION(BlueprintImplementableEvent)
	void DisappearWhenDead();

	// UI for showing how much health left
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY()
	USWorldUserWidget* ActiveHealthBar;

	UPROPERTY(EditAnywhere, Category = "UI")
	FVector HealthBarOffset;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
