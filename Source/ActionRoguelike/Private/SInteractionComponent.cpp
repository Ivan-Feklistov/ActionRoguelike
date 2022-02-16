// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGameplayInterface.h"
#include "DrawDebugHelpers.h"
#include "SCharacter.h"
#include "Camera/CameraComponent.h"




// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	InteractRange = 200.f;
	bDrawDebugLine = false;
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	
	FVector EyeLocation;
	FRotator EyeRotation;
	FVector End;
	AActor* MyOwner = GetOwner();
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	ASCharacter* MyCharacter = Cast<ASCharacter>(MyOwner);
	FHitResult LineHit;
	
	if (MyCharacter)
	{
		//EyeLocation = MyCharacter->CameraComp->GetComponentLocation();
		//EyeRotation = MyCharacter->CameraComp->GetForwardVector().Rotation();
		FVector LineEnd;
		
		LineEnd = MyCharacter->CameraComp->GetComponentLocation() + MyCharacter->CameraComp->GetForwardVector() * 10000;

		GetWorld()->LineTraceSingleByChannel(LineHit, MyCharacter->CameraComp->GetComponentLocation(), LineEnd, ECC_Visibility);

		
	}

	//End = EyeLocation + (EyeRotation.Vector() * InteractRange);
	End = EyeLocation + (LineHit.Location - EyeLocation).Rotation().Vector() * InteractRange;

	//FHitResult Hit;
	//GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	TArray<FHitResult> Hits;
	float Radius = 50.f;
	FCollisionShape Shape;
	Shape.SetSphere(30.f);
	bool bBlockingHit =  GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);

	FColor DebugColor = bBlockingHit ? FColor::Green : FColor::Red;

	if (bDrawDebugLine == true && bBlockingHit == false)
	{
		DrawDebugSphere(GetWorld(), End, Radius, 16, DebugColor, false, 2.f);
		DrawDebugLine(GetWorld(), EyeLocation, End, DebugColor, false, 2.f, 0, 2.f);
	}

	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			if (bDrawDebugLine)
			{
				DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 16, DebugColor, false, 2.f);
				DrawDebugLine(GetWorld(), EyeLocation, End, DebugColor, false, 2.f, 0, 2.f);
			}
			if (HitActor->Implements<USGameplayInterface>())
			{
				APawn* MyPawn = Cast<APawn>(MyOwner);
				ISGameplayInterface::Execute_Interact(HitActor, MyPawn);
				
				break;
			}
		}

	}
	
		
}

