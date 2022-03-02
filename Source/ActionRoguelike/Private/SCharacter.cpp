// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"

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

	DebugAttackHitLocation = false;

	AttackRate = 1.f;
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
		// Trace where camera looks, to determine where Projectile supposed to hit
		
		End = CameraComp->GetComponentLocation() + CameraComp->GetForwardVector() * 10000;
		
		GetWorld()->LineTraceSingleByChannel(Hit, CameraComp->GetComponentLocation(), End, ECC_Visibility);
		if (DebugAttackHitLocation)
			DrawDebugSphere(GetWorld(), Hit.Location, 12, 16, FColor::Red, false, 2.f);

		PlayAnimMontage(AttackAnim, AttackRate);
		GetWorldTimerManager().ClearTimer(TimerHandle_StartAttackAnim);
		if (Attack == Primary)
		{
			GetWorldTimerManager().SetTimer(TimerHandle_StartAttackAnim, this, &ASCharacter::PrimaryAttack, 0.2f / AttackRate);
		}
			
		if (Attack == Ultimate)
		{
			GetWorldTimerManager().SetTimer(TimerHandle_StartAttackAnim, this, &ASCharacter::UltimateAttack, 0.2f / AttackRate);
		}

		if (Attack == Dash)
		{
			GetWorldTimerManager().SetTimer(TimerHandle_StartAttackAnim, this, &ASCharacter::DashAttack, 0.2f / AttackRate);
		}
			
		TurnCharacterInDirectionOfAttack(CameraComp->GetForwardVector().Rotation());
	}

	
}


void ASCharacter::UltimateAttack()
{
	if (BlackHoleClass == nullptr)
	{
		return;
	}
	SpawnProjectile(BlackHoleClass);
}

void ASCharacter::DashAttack()
{
	if (DashClass == nullptr)
	{
		return;
	}
	SpawnProjectile(DashClass);
}

void ASCharacter::PrimaryAttack()
{
	if (PRojectileClass == nullptr)
	{
		return;
	}
	SpawnProjectile(PRojectileClass);
	
}

void ASCharacter::SpawnProjectile(TSubclassOf<AActor> ClassOfProjectile)
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	

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
	if ((HandLocation - CameraComp->GetComponentLocation()).Size() > (Hit.Location - CameraComp->GetComponentLocation()).Size())
	{
		RotOnTarget = (End - HandLocation).Rotation();
	}

	// Spawn Projectile
	FTransform SpawnTM = FTransform(RotOnTarget, HandLocation);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	GetWorld()->SpawnActor<AActor>(ClassOfProjectile, SpawnTM, SpawnParams);
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

	PlayerInputComponent->BindAction<FAttackAnimationDelegate>("PrimaryAttack", IE_Pressed, this, &ASCharacter::AttackAnimation, AttackType::Primary);
	PlayerInputComponent->BindAction<FAttackAnimationDelegate>("UltimateAttack", IE_Pressed, this, &ASCharacter::AttackAnimation, Ultimate);
	PlayerInputComponent->BindAction<FAttackAnimationDelegate>("DashAttack", IE_Pressed, this, &ASCharacter::AttackAnimation, Dash);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
}



void ASCharacter::OnHealthChanged(AActor* InstigatorACtor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (AttributeComp->Health > 0.f && Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials("HitFlashTime", GetWorld()->TimeSeconds);
	}

	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
	}
}



