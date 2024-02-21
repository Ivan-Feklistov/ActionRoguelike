// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"
#include "SGameMode.h"
#include "GameFramework/PlayerState.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("s.DamageMultiplier"), 1.f, TEXT("Global multiplier for damage"), ECVF_Cheat);

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Health = 100.f;
	MaxHealth = 100.f;
	Rage = 0.f;
	MaxRage = 100.f;
	MaxScore = 9999;
	bCanRage = false;
	GetPlayerScore();
}


// Called when the game starts
void USAttributeComponent::BeginPlay()
{
	Super::BeginPlay();


}


bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -MaxHealth);
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.f;
}


// when health changed broadcast it and check if dead
bool USAttributeComponent::ApplyHealthChange(AActor* HealthChangeInstigator, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	if (!IsAlive())
		return false;

	if (Delta < 0.0f)
	{
		Delta = Delta * CVarDamageMultiplier.GetValueOnGameThread();
	}
	
	float OldHealth = Health;

	Health += Delta;
	Health = FMath::Clamp(Health, 0.f, MaxHealth);
	float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(HealthChangeInstigator, this, Health, Delta);

	// died
	if (ActualDelta < 0.0f && Health == 0.0f)
	{
		ASGameMode* GameMode = GetWorld()->GetAuthGameMode<ASGameMode>();
		if (GameMode)
		{
			GameMode->OnActorKilled(GetOwner(), HealthChangeInstigator);
		}
	}

	return ActualDelta != 0;
}

bool USAttributeComponent::ApplyRageChange(AActor* RageChangeInstigator, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}
	
	if (!IsAlive())
		return false;

	if (!bCanRage)
		return false;
	
	const float OldRage = Rage;

	Rage = Rage + (Delta*RageFactor);
	Rage = FMath::Clamp(Rage, 0.f, MaxRage);
	const float ActualDelta = Rage - OldRage;
	OnRageChanged.Broadcast(RageChangeInstigator, this, Rage, Delta);

	return ActualDelta != 0;
}

bool USAttributeComponent::ApplyDamage(AActor* DamageInstigator, AActor* TargetActor, float Damage)
{
	if (USAttributeComponent* AttribComp = GetAttributes(TargetActor))
	{
		AttribComp->ApplyHealthChange(DamageInstigator, -Damage);
		AttribComp->ApplyRageChange(DamageInstigator, Damage);
		return true;
	}
	return false;
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}
	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}
	return false;
}

// credits

float USAttributeComponent::GetPlayerScore()
{
	APawn* Player = Cast<APawn>(GetOwner());
	if (Player)
	{
		APlayerState* PlayerState = Player->GetPlayerState();
		if (PlayerState)
		{	
			PlayerScore = PlayerState->GetScore();
			return PlayerScore;
		}
	}
	return 0;

}


void USAttributeComponent::ChangePlayerScore(float DeltaScore)
{
	APawn* Player = Cast<APawn>(GetOwner());
	if (Player)
	{
		APlayerState* PlayerState = Player->GetPlayerState();
		if (PlayerState)
		{
			GetPlayerScore();
			float OldScore = PlayerScore;
			PlayerScore = PlayerScore + DeltaScore;
			PlayerScore = FMath::Clamp(PlayerScore, 0.0f, MaxScore);
			PlayerState->SetScore(PlayerScore);

			float ActualDelta = PlayerScore - OldScore;
			OnScoreChanged.Broadcast(this, PlayerScore, ActualDelta);
		}

	}
}

