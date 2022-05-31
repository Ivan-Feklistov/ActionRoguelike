// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"
#include "SCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USInteractionComponent;
class UAnimMontage;
class USAttributeComponent;
class UCameraShakeBase;
class UParticleSystem;
class USActionComponent;

UENUM(BlueprintType)
enum AttackType
{
	Primary,
	Ultimate,
	Dash
};

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	UFUNCTION(BlueprintImplementableEvent)
	void TurnCharacterInDirectionOfAttack(FRotator FaceOrientation);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USAttributeComponent* AttributeComp;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase> ShakeEffect;

	UPROPERTY(EditAnywhere)
	float InRadius;

	UPROPERTY(EditAnywhere)
	float OutRadius;
	

// 	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AttackType")
// 	AttackType Attack;
	

protected:

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Action")
	USActionComponent* ActionComp;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void AttackAnimation(AttackType Attack);
	DECLARE_DELEGATE_OneParam(FAttackAnimationDelegate, AttackType);
	
	UFUNCTION()
	void PrimaryInteract();

	UFUNCTION()
	void SprintStart();

	UFUNCTION()
	void SprintStop();

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION()
	void Killed();

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100.f);

};
