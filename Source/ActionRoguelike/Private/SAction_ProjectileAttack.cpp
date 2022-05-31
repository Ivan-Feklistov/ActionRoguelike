// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ProjectileAttack.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	AttackAnimDelay = 0.2f;
	AttackRate = 1.f;
	HandSocketName = "Muzzle_01";
	DebugAttackHitLocation = false;
}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	ACharacter* Character = Cast<ACharacter>(InstigatorActor);
	if (Character)
	{
		// play attack animation
		if (AttackAnim)
			Character->PlayAnimMontage(AttackAnim, AttackRate);
		
		// set timer to activate attack
		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);

	}	
}


void USAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (ProjectileClass)
	{
		FHitResult Hit;
		// FVector Start = InstigatorCharacter->GetPawnViewLocation();
		FVector Start;
		FRotator StartRot;
		InstigatorCharacter->GetController()->GetPlayerViewPoint(Start, StartRot);
		// FVector End = Start + (InstigatorCharacter->GetControlRotation().Vector() * 10000);
		FVector End = Start + (StartRot.Vector() * 10000);
		FCollisionQueryParams QueryParams;
		QueryParams.bTraceComplex = true;
		QueryParams.AddIgnoredActor(InstigatorCharacter);

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		GetWorld()->SweepSingleByObjectType(Hit, Start, End, FQuat::Identity, ObjParams, Shape, QueryParams);

		// OLD GetWorld()->LineTraceSingleByChannel(Hit, CameraComp->GetComponentLocation(), End, ECC_Camera, QueryParams);

		if (DebugAttackHitLocation)
			DrawDebugSphere(GetWorld(), Hit.Location, 20, 16, FColor::Red, false, 2.f);

		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);
	
		// Rotation of projectile on spawn
		FRotator RotOnTarget;
		if (Hit.bBlockingHit)
		{
			RotOnTarget = (Hit.Location - HandLocation).Rotation();
		}
		else
		{
			RotOnTarget = (End - HandLocation).Rotation();
		}
		// if Camera look at something between camera and player i.e. Something blocking view
		if ((HandLocation - Start).Size() > (Hit.Location - Start).Size())
		{
			RotOnTarget = (End - HandLocation).Rotation();
		}
	
		// Spawn Projectile
		FTransform SpawnTM = FTransform(RotOnTarget, HandLocation);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);

		// spawn muzzle effect
		if (MuzzleEffect)
			UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, InstigatorCharacter->GetMesh(), HandSocketName,
				FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
	}

	StopAction(InstigatorCharacter);
}

