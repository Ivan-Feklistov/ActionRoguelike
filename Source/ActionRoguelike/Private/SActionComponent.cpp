// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionComponent.h"

// Sets default values for this component's properties
USActionComponent::USActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<USAction> ActionClass : DefaultActions)
	{
		AddAction(GetOwner(), ActionClass);
	}
	
}


// Called every frame
void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// debug - what action is active
	if (DebugMssages)
	{
		FString DebugMessage = "Actor: " + GetNameSafe(GetOwner()) + " is executing action: " + ActiveGameplayTags.ToStringSimple();
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMessage);
	}

}

void USActionComponent::AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass)
{
	if (!ActionClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot add Action: nullptr"));
		return;
	}
	
	USAction* NewAction = NewObject<USAction>(this, ActionClass);
	if (NewAction)
	{
		for (USAction* Action : Actions)
		{
			if (Action->ActionName == NewAction->ActionName)
			{
				UE_LOG(LogTemp, Warning, TEXT("Player already has that Action: %s"), *GetNameSafe(ActionClass));
				return;
			}
		}

		Actions.Add(NewAction);

		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

void USActionComponent::RemoveAction(USAction* ActionToRemove)
{
	if (!ActionToRemove)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot remove Action: nullptr"));
		return;
	}

	if (ActionToRemove->IsRunning())
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot remove Action: Action is running"));
		return;
	}

	for (USAction* Action : Actions)
	{
		if (Action->ActionName == ActionToRemove->ActionName)
		{	
			Actions.Remove(Action);
			break;
		}	
	}
	UE_LOG(LogTemp, Warning, TEXT("Player doesn't have that Action: %s"), *GetNameSafe(ActionToRemove));
}

bool USActionComponent::StartActionByName(AActor* InstigatorActor, FName ActionName)
{
	for (USAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(InstigatorActor))
			{
				continue;
			}
			Action->StartAction(InstigatorActor);
			return true;
		}
	}
	return false;
}

bool USActionComponent::StopActionByName(AActor* InstigatorActor, FName ActionName)
{
	for (USAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->IsRunning())
			{
				Action->StopAction(InstigatorActor);
				return true;
			}			
		}
	}
	return false;
}

