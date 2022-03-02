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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DebugAttackHitLocation;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRate;

	UFUNCTION(BlueprintImplementableEvent)
	void TurnCharacterInDirectionOfAttack(FRotator FaceOrientation);

	

	

// 	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AttackType")
// 	AttackType Attack;
	

protected:

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void AttackAnimation(AttackType Attack);
	DECLARE_DELEGATE_OneParam(FAttackAnimationDelegate, AttackType);

	void PrimaryAttack();
	void UltimateAttack();
	void DashAttack();
	void PrimaryInteract();

	UFUNCTION()
	void SpawnProjectile(TSubclassOf<AActor> ClassOfProjectile);

	FTimerHandle TimerHandle_StartAttackAnim;

	FVector End;
	FHitResult Hit;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> PRojectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> BlackHoleClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> DashClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USAttributeComponent* AttributeComp;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorACtor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
