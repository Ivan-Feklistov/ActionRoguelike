// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "SActionComponent.h"


bool USAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}

	USActionComponent* Comp = GetActionComponent();
	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}

	if (bUseCostRage)
	{
		USAttributeComponent* AttribComp = USAttributeComponent::GetAttributes(Instigator);
		if (AttribComp != nullptr)
		{
			if (AttribComp->Rage < CostRage)
				return false;
		}
	}
	return true;
}

void USAction::StartAction_Implementation(AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));

	USActionComponent* Comp = GetActionComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	if (bUseCostRage)
	{
		USAttributeComponent* AttribComp = USAttributeComponent::GetAttributes(InstigatorActor);
		if (AttribComp != nullptr)
		{
			AttribComp->ApplyRageChange(InstigatorActor, -CostRage);
				
		}
	}
	bIsRunning = true;
}

void USAction::StopAction_Implementation(AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));

	ensureAlways(bIsRunning);

	USActionComponent* Comp = GetActionComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
}

// outer is set when creating NewObject<T>
UWorld* USAction::GetWorld() const
{
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if (Comp)
	{
		return Comp->GetWorld();
	}
	return nullptr;
}

USActionComponent* USAction::GetActionComponent() const
{
	return Cast<USActionComponent>(GetOuter());
}