// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Delegates/Delegate.h"
#include "SAttributeComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewHealth, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnScoreChanged, USAttributeComponent*, OwningComp, float, NewScore, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	// return pointer to attribute component of an actor if he has one
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static USAttributeComponent* GetAttributes(AActor* FromActor);

	// if Actor has attribute component and health > 0 then return true, and return false if he's dead or doesn't have attribute component
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static bool IsActorAlive(AActor* Actor);

	// Sets default values for this component's properties
	USAttributeComponent();

protected:


	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsAlive() const;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	// for both heal and damage
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* HealthChangeInstigator, float Delta);

	// static for damaging
	UFUNCTION(BlueprintCallable, Category = "SGameplayStatics")
	static bool ApplyDamage(AActor* DamageInstigator, AActor* TargetActor, float Damage);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Credits")
	float PlayerScore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Credits")
	float MaxScore;

	UFUNCTION(BlueprintCallable, Category = "Credits")
	float GetPlayerScore();

	UFUNCTION(BlueprintCallable, Category = "Credits")
	void ChangePlayerScore(float DeltaScore);

	UPROPERTY(BlueprintAssignable)
	FOnScoreChanged OnScoreChanged;
};
