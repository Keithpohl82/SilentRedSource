// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UserCharacter.generated.h"


class UCameraComponent;
class USpringArmComponent;
class AARBaseWeapon;
class UPlayerHealthComponent;

UCLASS()
class SILENTRED2_API AUserCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUserCharacter();

	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	bool bIsCarringFlag;

	

	


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void BeginCrouch();
	void EndCrouch();

	void NextWeapon();
	void PreviousWeapon();




	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Weapon")
	AARBaseWeapon* CurrentWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPlayerHealthComponent* HealthComp;

	UPROPERTY(EditDefaultsOnly, Category = "Player|Weapons")
	TSubclassOf<AARBaseWeapon> StarterWeaponClass;

	UPROPERTY(VisibleDefaultsOnly, Category = "Player|Weapon")
	FName WeaponAttachSocketName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapons")
	TArray<TSubclassOf<AARBaseWeapon>> WeaponLoadout;


	void StartFire();

	void StopFire();


	UFUNCTION()
	void Reload();


	UFUNCTION()
	void OnHealthChanged(UPlayerHealthComponent* OwningHealthComp, float Health, float Armor, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	
	void RespawnPlayer();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	bool bDied;

};
