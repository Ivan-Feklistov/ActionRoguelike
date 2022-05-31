// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "SAttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SCharacter.h"
#include "SActionComponent.h"



// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LifeSpan = 5.f;
	Damage = 20.f;
	bCanDealDamage = true;
}



// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (AudioComp->Sound != nullptr)
	{
		AudioComp->Play();
	}
	SetIgnores(GetInstigator());
}


void ASMagicProjectile::SetIgnores(AActor* InstigatorActor)
{
	SphereComp->IgnoreActorWhenMoving(InstigatorActor, true);
	SphereComp->IgnoreActorWhenMoving(this, true);
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
 	Super::OnOverlapEvent();

	if (OtherActor == nullptr || OtherActor == GetInstigator())
	{
		return;
	}

	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	if (AttributeComp && bCanDealDamage)
	{

		USActionComponent* ActionComp = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Parry!"));
			MovementComp->Velocity = -MovementComp->Velocity;
			SphereComp->IgnoreActorWhenMoving(GetInstigator(), false);
			SetInstigator(Cast<APawn>(OtherActor));
			SetIgnores(OtherActor);
			return;
		}
		AttributeComp->ApplyHealthChange(GetInstigator(), -Damage);
	}

	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, SweepResult.Location);
	}
	Destroy();
}

void ASMagicProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//Super::OnHitEvent();

// 	if (OtherActor == nullptr || OtherActor == GetInstigator())
// 	{
// 		return;
// 	}
// 
// 	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
// 	if (AttributeComp && bCanDealDamage)
// 	{
// 
// 		USActionComponent* ActionComp = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
// 		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
// 		{
// 			UE_LOG(LogTemp, Warning, TEXT("Parry!"));
// 			MovementComp->Velocity = -MovementComp->Velocity;
// 			SphereComp->IgnoreActorWhenMoving(GetInstigator(), false);
// 			SetInstigator(Cast<APawn>(OtherActor));
// 			SetIgnores(OtherActor);
// 			return;
// 		}
// 		AttributeComp->ApplyHealthChange(GetInstigator(), -Damage);
// 	}
// 
// 	if (ImpactSound)
// 	{
// 		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Hit.Location);
// 	}	
// 	Destroy();
}

// Called every frame
void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

