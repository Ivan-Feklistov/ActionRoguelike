// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "DrawDebugHelpers.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	bDebugPlayerSpotted = false;
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
}

// Called every frame
void ASAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
// if ai pawn sees player, then set blackboard 'target' value
void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	AAIController* AICont = Cast<AAIController>(GetController());
	if (AICont)
	{
		UBlackboardComponent* BBComp = AICont->GetBlackboardComponent();

		BBComp->SetValueAsObject("Target", Pawn);
		//debug message
		if (bDebugPlayerSpotted)
		{
			DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.f, true);
		}
		
	}
}


