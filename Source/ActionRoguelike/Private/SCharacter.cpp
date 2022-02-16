// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SInteractionComponent.h"
#include "DrawDebugHelpers.h"

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

	DebugAttackHitLocation = false;

	AttackRate = 1.f;
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

void ASCharacter::PrimaryAttack()
{
	if(GetMesh()->GetAnimInstance()->Montage_IsPlaying(AttackAnim) == false)
	{
		PlayAnimMontage(AttackAnim, AttackRate);
		GetWorldTimerManager().ClearTimer(TimerHandle_StartAttackAnim);
		GetWorldTimerManager().SetTimer(TimerHandle_StartAttackAnim, this, &ASCharacter::PrimaryAttackAnim, 0.2f / AttackRate);
		TurnCharacterInDirectionOfAttack(CameraComp->GetForwardVector().Rotation());
	}

	
}



void ASCharacter::PrimaryAttackAnim()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	//FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);

	FVector End;
	End = CameraComp->GetComponentLocation() + CameraComp->GetForwardVector() * 10000;
	FHitResult Hit;
	// Trace where camera looks, to determine where Projectile supposed to hit
	GetWorld()->LineTraceSingleByChannel(Hit, CameraComp->GetComponentLocation(), End, ECC_Visibility);
	if (DebugAttackHitLocation)
		DrawDebugSphere(GetWorld(), Hit.Location, 12, 16, FColor::Red, false, 2.f);

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
	if ((HandLocation - CameraComp->GetComponentLocation()).Size() > (Hit.Location - CameraComp->GetComponentLocation()).Size())
	{
		RotOnTarget = (End - HandLocation).Rotation();
	}

	FTransform SpawnTM = FTransform(RotOnTarget, HandLocation);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	GetWorld()->SpawnActor<AActor>(PRojectileClass, SpawnTM, SpawnParams);
}

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

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
}

