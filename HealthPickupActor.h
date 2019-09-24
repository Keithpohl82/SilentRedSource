// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthPickupActor.generated.h"

UCLASS()
class SILENTRED2_API AHealthPickupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealthPickupActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	float PickupInterval;

	/**/
	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	int32 TotalNrOfTicks;

	

	FTimerHandle TimerHandle_PickupTick;

	int32 TicksProcessed;

	UFUNCTION()
	void OnTickPickup();

	UPROPERTY(ReplicatedUsing = OnRep_HealthPickupActive)
	bool bIsHealthPickupActive;

	UFUNCTION()
	void OnRep_HealthPickupActive();

	UFUNCTION(BlueprintImplementableEvent, Category = "Pickup")
	void OnHealthPickupActiveStateChanged(bool bNewIsActive);


public:	

	void ActivatePickup(AActor* ActiveFor);

	UFUNCTION(BlueprintImplementableEvent, Category = "Pickup")
	void OnActivated(AActor* ActiveFor);

	UFUNCTION(BlueprintImplementableEvent, Category = "Pickup")
	void OnPickupTicked();

	UFUNCTION(BlueprintImplementableEvent, Category = "Pickup")
	void OnExpired();

};
