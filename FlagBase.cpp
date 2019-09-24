// Fill out your copyright notice in the Description page of Project Settings.


#include "FlagBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "UserCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFlagBase::AFlagBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp->SetupAttachment(MeshComp);

}

// Called when the game starts or when spawned
void AFlagBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFlagBase::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, PickupFX, GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
}



void AFlagBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	PlayEffects();

	AUserCharacter* MyCharacter = Cast<AUserCharacter>(OtherActor);
	if (MyCharacter)
	{
		MyCharacter->bIsCarringFlag = true;

		Destroy();
	}
}

