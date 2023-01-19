// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGameplayInterface.h"
#include "DrawDebugHelpers.h"
#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "SWorldUserWidget.h"




// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	InteractRange = 200.f;
	InteractionRadius = 30.0f;
	CollisionChannel = ECC_WorldDynamic;
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

	FindBestInteractable();
}

void USInteractionComponent::FindBestInteractable()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

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
	//float Radius = 50.f;
	FCollisionShape Shape;
	Shape.SetSphere(InteractionRadius);
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);

	FColor DebugColor = bBlockingHit ? FColor::Green : FColor::Red;

	if (bDrawDebugLine == true && bBlockingHit == false)
	{
		DrawDebugSphere(GetWorld(), End, InteractionRadius, 16, DebugColor, false, 2.f);
		DrawDebugLine(GetWorld(), EyeLocation, End, DebugColor, false, 2.f, 0, 2.f);
	}

	// clear ref before trying to fill
	FocusedActor = nullptr;

	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			if (bDrawDebugLine)
			{
				DrawDebugSphere(GetWorld(), Hit.ImpactPoint, InteractionRadius, 16, DebugColor, false, 2.f);
				DrawDebugLine(GetWorld(), EyeLocation, End, DebugColor, false, 2.f, 0, 2.f);
			}
			if (HitActor->Implements<USGameplayInterface>())
			{
				FocusedActor = HitActor;

				break;
			}
		}

	}

	if (FocusedActor)
	{
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;

			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}

	}
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}
}


void USInteractionComponent::PrimaryInteract()
{
	if (FocusedActor == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(103, 1.0f, FColor::Red, FString::Printf(TEXT("No focus Actor to interact")));
		return;
	}

	APawn* MyPawn = Cast<APawn>(GetOwner());
	ISGameplayInterface::Execute_Interact(FocusedActor, MyPawn);
		
}

