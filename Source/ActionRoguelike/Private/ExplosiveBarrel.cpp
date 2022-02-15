// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveBarrel.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "SMagicProjectile.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->BodyInstance.bNotifyRigidBodyCollision = true;
	RootComponent = MeshComp;

	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(MeshComp);

}

// Called when the game starts or when spawned
void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();

	MeshComp->OnComponentHit.AddDynamic(this, &AExplosiveBarrel::Explode);
	
}

void AExplosiveBarrel::Explode(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (MeshComp->BodyInstance.bNotifyRigidBodyCollision == false)
	{
		return;
	}
	if (OtherActor->GetClass()->IsChildOf(ASMagicProjectile::StaticClass()))
	{
		ForceComp->FireImpulse();
		if (Effect != nullptr)
		{
			FTransform SpawnTM = FTransform(GetActorRotation(), Hit.Location);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Effect, SpawnTM);
		}
		
		Destroy();
	}

		
}

// Called every frame
void AExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

