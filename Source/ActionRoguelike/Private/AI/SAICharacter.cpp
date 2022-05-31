// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "AI/SAIController.h"
#include "BrainComponent.h"
#include "SCharacter.h"
#include "Components/CapsuleComponent.h"
#include "SWorldUserWidget.h"
#include "SActionComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");
	bDebugPlayerSpotted = false;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	ForceDirection = FVector(-100.f, 0.f, 0.f);
	HealthBarOffset = FVector(0.f, 0.f, 0.f);

	ScoreCost = 10;
}


USAttributeComponent* ASAICharacter::GetAttributeComponent()
{
	if (AttributeComp)
	{
		return AttributeComp;
	}
	return nullptr;
}


// Called when the game starts or when spawned
void ASAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

// Called every frame
void ASAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
// if ai pawn sees player, then set blackboard 'target' value as player
void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetNewTargetActor(Pawn);
	//debug message
	if (bDebugPlayerSpotted && Pawn == GetController()->GetPawn())
	{
		DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.f, true);
	}
}

// set new target to shoot for AI
void ASAICharacter::SetNewTargetActor(AActor* InstigatorActor)
{
	AAIController* AICont = Cast<AAIController>(GetController());
	if (AICont)
	{
		UBlackboardComponent* BBComp = AICont->GetBlackboardComponent();

		BBComp->SetValueAsObject("Target", InstigatorActor);
	}
}

// if bot damaged by player then spawn healthbar over bot, flash red on material, and destroy if killed
void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f && InstigatorActor->GetClass()->IsChildOf(ASCharacter::StaticClass()))
	{
		// spawn healbar
		if (ActiveHealthBar == nullptr && HealthBarWidgetClass)
		{
			ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->WorldOffset = HealthBarOffset;
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}

		//UE_LOG(LogTemp, Log, TEXT("Minion Damaged"));
		SetNewTargetActor(InstigatorActor);
		
		// flash red on Ai mesh material
		GetMesh()->SetScalarParameterValueOnMaterials("HitFlashTime", GetWorld()->TimeSeconds);

		// if dead
		if (NewHealth <= 0.0f)
		{
			// stop BT
			ASAIController* AICont = Cast<ASAIController>(GetController());
			{
				if (AICont)
				{
					AICont->GetBrainComponent()->StopLogic("Killed");
				}
			}
			// ragdoll
			
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			
			// destroy timer
			SetLifeSpan(15.f);
			// blueprint event for showing body disappearance
			DisappearWhenDead();
			// launch dead body away from player
			GetMesh()->AddImpulse(ForceDirection, NAME_None, true);
		}
	}
}



