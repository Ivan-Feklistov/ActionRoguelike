// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashAbility.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASDashAbility::ASDashAbility()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	LifeSpan = 2.f;
	TimeUntilTeleport = 1.f;

}

// Called when the game starts or when spawned
void ASDashAbility::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(TimerHandle_Explode, this, &ASDashAbility::Explode, TimeUntilTeleport);
	//SphereComp->OnComponentHit.AddDynamic(this, &ASDashAbility::OnHit);
}

void ASDashAbility::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

void ASDashAbility::Explode()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Explode);
	EffectComp->Complete();

	MovementComp->StopMovementImmediately();

	EffectLocation = GetActorLocation();
	EffectRotation = (GetInstigator()->GetActorLocation() - EffectLocation).Rotation();
	if (Explosion != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, EffectLocation, EffectRotation);
	}
	
	FTimerHandle TimerHandle_Destroy;

	AActor* Player = GetInstigator();
	FVector NewLocation = EffectLocation - FVector(0.f, 0.f, 90.f);
	if (NewLocation.Z < 0.f)
	{
		NewLocation.Z = 0.f;
	}
	UE_LOG(LogTemp, Log, TEXT("New location of Player is: %s"), *NewLocation.ToString());
	// Teleport Player
	//Player->SetActorLocation(NewLocation);
	Player->TeleportTo(EffectLocation, Player->GetActorRotation(), false, false);

	GetWorldTimerManager().SetTimer(TimerHandle_Destroy, this, &ASDashAbility::DestroyMe, 0.2f);
}

void ASDashAbility::DestroyMe()
{
	Destroy();
}



// Called every frame
void ASDashAbility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

