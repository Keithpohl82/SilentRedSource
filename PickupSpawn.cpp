// Fill out your copyright notice in the Description page of Project Settings.
//PickupActor


#include "PickupSpawn.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "HealthPickupActor.h"
#include "TimerManager.h"



// Sets default values
APickupSpawn::APickupSpawn()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(75.0f);
	RootComponent = SphereComp;


	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->SetRelativeRotation(FRotator(90, 0.0f, 0.0f));
	DecalComp->DecalSize = FVector(64, 75, 75);
	DecalComp->SetupAttachment(RootComponent);

	CoolDownDuration = 10.0f;

	SetReplicates(true);
}

// Called when the game starts or when spawned
void APickupSpawn::BeginPlay()
{
	Super::BeginPlay();

	if (Role == ROLE_Authority)
	{
		Respawn();
	}

}

void APickupSpawn::Respawn()
{

	if (PickupClass == nullptr)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	PickupInstance = GetWorld()->SpawnActor<AHealthPickupActor>(PickupClass, GetTransform(), SpawnParams);
}

void APickupSpawn::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (Role == ROLE_Authority && PickupInstance)
	{
		PickupInstance->ActivatePickup(OtherActor);
		PickupInstance = nullptr;

		GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &APickupSpawn::Respawn, CoolDownDuration);
	}
}



