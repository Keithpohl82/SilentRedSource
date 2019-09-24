// Fill out your copyright notice in the Description page of Project Settings.
//PowerupActor

#include "HealthPickupActor.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AHealthPickupActor::AHealthPickupActor()
{
	PickupInterval = 0.0f;
	TotalNrOfTicks = 0.0f;
	
	bIsHealthPickupActive = false;

	SetReplicates(true);
}

// Called when the game starts or when spawned
void AHealthPickupActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (Role == ROLE_Authority)
	{
		
	}
	
}

void AHealthPickupActor::OnTickPickup()
{
	TicksProcessed++;

	OnPickupTicked();

	if (TicksProcessed >= TotalNrOfTicks)
	{
		OnExpired();

		bIsHealthPickupActive = false;
		OnRep_HealthPickupActive();

		GetWorldTimerManager().ClearTimer(TimerHandle_PickupTick);
	}
}

void AHealthPickupActor::OnRep_HealthPickupActive()
{
	OnHealthPickupActiveStateChanged(bIsHealthPickupActive);
}

void AHealthPickupActor::ActivatePickup(AActor* ActiveFor)
{
	OnActivated(ActiveFor);

	bIsHealthPickupActive = true;
	OnRep_HealthPickupActive();

	if (PickupInterval > 0.0f)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_PickupTick, this, &AHealthPickupActor::OnTickPickup, PickupInterval, true, 0.0f);
	}
	else
	{
		OnTickPickup();
	}
}


void AHealthPickupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const

{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHealthPickupActor, bIsHealthPickupActive);
	
}