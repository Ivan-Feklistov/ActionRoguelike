// Fill out your copyright notice in the Description page of Project Settings.


#include "SPotion.h"
#include "SCharacter.h"
#include "SAttributeComponent.h"

// Sets default values
ASPotion::ASPotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bCanInteract = true;
	HealthCapacity = 20.f;
}

// Called when the game starts or when spawned
void ASPotion::BeginPlay()
{
	Super::BeginPlay();
	
}


void ASPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (bCanInteract)
	{
		ASCharacter* Player = Cast<ASCharacter>(InstigatorPawn);
		USAttributeComponent* AttributeComp = Player->AttributeComp;
		if (AttributeComp)
		{
			if (AttributeComp->Health == AttributeComp->MaxHealth)
			{
				return;
			}
	
			//add health to character
			AttributeComp->ApplyHealthChange(HealthCapacity);
			
			// hide mesh for 10 sec
			SetActorHiddenInGame(true);
			bCanInteract = false;
			UE_LOG(LogTemp, Warning, TEXT("Potion interacted!"));
			GetWorldTimerManager().SetTimer(TimerHandle_ResetTimer, this, &ASPotion::ResetPotion, 10);
		}
	}
	
}


void ASPotion::ResetPotion()
{
	bCanInteract = true;
	SetActorHiddenInGame(false);
}

// Called every frame
void ASPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

