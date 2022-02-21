// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SMagicProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;


UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASMagicProjectile();

	UPROPERTY(EditAnywhere)
	float LifeSpan;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UProjectileMovementComponent* MovementComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UParticleSystemComponent* EffectComp;

	void SetupBaseConstructor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
