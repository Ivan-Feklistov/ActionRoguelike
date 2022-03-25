// Fill out your copyright notice in the Description page of Project Settings.


#include "SBasePickUp.h"



// Sets default values
ASBasePickUp::ASBasePickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;
}

// Called when the game starts or when spawned
void ASBasePickUp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASBasePickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASBasePickUp::Interact_Implementation(APawn* InstigatorPawn)
{

}



