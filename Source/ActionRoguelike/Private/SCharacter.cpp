// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SActionComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");

}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.f;
	ControlRot.Roll = 0.f;
	AddMovementInput(ControlRot.Vector(), Value);
}

void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.f;
	ControlRot.Roll = 0.f;

	FVector ControlRightVec = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(ControlRightVec, Value);
}

// Play animation of attack, and then run required attack function
void ASCharacter::AttackAnimation(AttackType Attack)
{
	if(GetMesh()->GetAnimInstance()->Montage_IsPlaying(AttackAnim) == false)
	{

		//GetWorldTimerManager().ClearTimer(TimerHandle_StartAttackAnim);
		if (Attack == Primary)
		{
			//GetWorldTimerManager().SetTimer(TimerHandle_StartAttackAnim, this, &ASCharacter::PrimaryAttack, 0.2f / AttackRate);
			ActionComp->StartActionByName(this, "PrimaryAttack");
		}
			
		if (Attack == Ultimate)
		{
			//GetWorldTimerManager().SetTimer(TimerHandle_StartAttackAnim, this, &ASCharacter::UltimateAttack, 0.2f / AttackRate);
			ActionComp->StartActionByName(this, "Ultimate");
		}

		if (Attack == Dash)
		{
			//GetWorldTimerManager().SetTimer(TimerHandle_StartAttackAnim, this, &ASCharacter::DashAttack, 0.2f / AttackRate);
			ActionComp->StartActionByName(this, "Dash");
		}
			
		TurnCharacterInDirectionOfAttack(CameraComp->GetForwardVector().Rotation());
	}

	
}


// call interaction on interaction component
void ASCharacter::PrimaryInteract()
{
	InteractionComp->PrimaryInteract();
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction<FAttackAnimationDelegate>("PrimaryAttack", IE_Pressed, this, &ASCharacter::AttackAnimation, AttackType::Primary);
	PlayerInputComponent->BindAction<FAttackAnimationDelegate>("UltimateAttack", IE_Pressed, this, &ASCharacter::AttackAnimation, Ultimate);
	PlayerInputComponent->BindAction<FAttackAnimationDelegate>("DashAttack", IE_Pressed, this, &ASCharacter::AttackAnimation, Dash);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASCharacter::SprintStop);
}

void ASCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void ASCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}


void ASCharacter::HealSelf(float Amount /* = 100*/)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (AttributeComp->Health > 0.f && Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials("HitFlashTime", GetWorld()->TimeSeconds);
		// shake camera
		UE_LOG(LogTemp, Log, TEXT("Player damaged"));
		if (ShakeEffect)
		{
			UGameplayStatics::PlayWorldCameraShake(GetWorld(), ShakeEffect, GetActorLocation(), InRadius, OutRadius);
		}
	}
	// if dead stop movement and collisions
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
		UCapsuleComponent* Capsule = GetCapsuleComponent();
		Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
		USkeletalMeshComponent* MeshComp = GetMesh();
		MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		FTimerHandle TimerHandle_Destroy;
		GetWorldTimerManager().SetTimer(TimerHandle_Destroy, this, &ASCharacter::Killed, 5.f);
	}
}

void ASCharacter::Killed()
{
	Destroy();
}


