// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SMagicProjectile.h"

#include "SDashAbility.generated.h"

class UParticleSystem;

UCLASS()
class ACTIONROGUELIKE_API ASDashAbility : public ASMagicProjectile
{
	GENERATED_BODY()
	
public:	
	//ASDashAbility() : ASMagicProjectile("SetupClass"){}
	// Sets default values for this actor's properties
	ASDashAbility();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float TimeUntilTeleport;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Explode();

	void DestroyMe();

	//UFUNCTION()
	//void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

	FTimerHandle TimerHandle_Explode;

	FVector EffectLocation;
	FRotator EffectRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* Explosion;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
