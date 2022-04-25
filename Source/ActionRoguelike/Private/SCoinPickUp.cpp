// Fill out your copyright notice in the Description page of Project Settings.


#include "SCoinPickUp.h"
#include "SCharacter.h"
#include "SAttributeComponent.h"

ASCoinPickUp::ASCoinPickUp()
{
	bCanInteract = true;
	ResetTime = 10.f;
	CoinCost = 5.f;
}


void ASCoinPickUp::BeginPlay()
{
	Super::BeginPlay();

}

void ASCoinPickUp::OnPickUpInteract_Implementation(APawn* InstigatorPawn)
{
	ASCharacter* Player = Cast<ASCharacter>(InstigatorPawn);
	USAttributeComponent* AttributeComp = Player->AttributeComp;
	if (AttributeComp)
	{
		// increase credits
		AttributeComp->ChangePlayerScore(CoinCost);

		// hide mesh for 10 sec
		HIDE
		UE_LOG(LogTemp, Log, TEXT("Coin interacted!"));
		RESET
	}
}