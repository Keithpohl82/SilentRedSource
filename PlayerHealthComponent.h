// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerHealthComponent.generated.h"

//On Health changed event
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SevenParams(FOnHealthChangedSignature, UPlayerHealthComponent*, HealthComp, float, Armor, float, Health, float, HealthDelta, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*,DamageCauser);

UCLASS( ClassGroup=(FPS), meta=(BlueprintSpawnableComponent) )
class SILENTRED2_API UPlayerHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	bool bIsDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthComponent")
	float DefaultArmor;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "HealthComponent")
	float Armor;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "HealthComponent")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthComponent")
	float DefaultHealth;

	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamageActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(BlueprintReadOnly, Category = "HealthComponent")
	bool hasArmor;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "HealthComponent")
	float HealthToAdd = 100.0f;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "HealthComponent")
	float ArmourToAdd = 100.0f;

public:	
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChangedSignature OnHealthChanged;

	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	void AddHealth(float HealAmount);

	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	void AddArmor(float ArmorAdded);
		
};
