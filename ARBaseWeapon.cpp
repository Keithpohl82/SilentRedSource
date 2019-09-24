// Fill out your copyright notice in the Description page of Project Settings.


#include "ARBaseWeapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "SilentRed2.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"



static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDebugWeaponDrawing(TEXT("DebugWeaponDrawing"), DebugWeaponDrawing, TEXT("Weapon Debug Draw Line Trace"), ECVF_Cheat);



// Sets default values
AARBaseWeapon::AARBaseWeapon()
{
 	

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	MuzzleSocketName = "Muzzle";

	BaseDamage = 20.0f;

	RateOfFire = 600;

	Ammo = 0;

	ClipSize = 0;

	AmmoPool = 0;

	SetReplicates(true);
}

void AARBaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	TimeBetweenShots = 60 / RateOfFire;
}

void AARBaseWeapon::Fire()
{

	if (Role < ROLE_Authority)
	{
		ServerFire();
	}

	if (Ammo <= 0)
	{
		return;
	}

	Ammo = Ammo - 1;

	//Trace the world from pawn eyes to crosshair location
	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector ShotDirection = EyeRotation.Vector();
		FVector TraceEnd = EyeLocation + (ShotDirection * 10000);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;

		FVector TracerEndPoint = TraceEnd;

		EPhysicalSurface SurfaceType = SurfaceType_Default;

		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_WEAPON, QueryParams))
		{
			//Process damage if blocking hit.
			AActor* HitActor = Hit.GetActor();

			SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

			float ActualDamage = BaseDamage;

			if (SurfaceType == SURFACE_PLAYERUNARMORED)
			{
				ActualDamage *= 3.0f;
			}

			UGameplayStatics::ApplyPointDamage(HitActor, ActualDamage, ShotDirection, Hit, MyOwner->GetInstigatorController(), this, DamageType);

			PlayImpactEffects(SurfaceType, Hit.ImpactPoint);
			

			TracerEndPoint = Hit.ImpactPoint;

			
		}
		if (DebugWeaponDrawing > 0)
		{
			DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::White, false, 0.5f, 0, 1.0f);
		}
		PlayFireEffects(TracerEndPoint);

		if (Role == ROLE_Authority)
		{
			HitScanTrace.TraceTo = TracerEndPoint;
			HitScanTrace.SurfaceType = SurfaceType;
		}

		LastFiredTime = GetWorld()->TimeSeconds;
	}
}

void AARBaseWeapon::ServerFire_Implementation()
{
	Fire();
}

bool AARBaseWeapon::ServerFire_Validate()
{
	return true;
}

void AARBaseWeapon::OnRep_HitScanTrace()
{
	PlayFireEffects(HitScanTrace.TraceTo);
	PlayImpactEffects(HitScanTrace.SurfaceType, HitScanTrace.TraceTo);
}

void AARBaseWeapon::StartFire()
{
	float FirstDelay = FMath::Max(LastFiredTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);

	GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenShots, this, &AARBaseWeapon::Fire, TimeBetweenShots, true, FirstDelay);
}

void AARBaseWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
}

void AARBaseWeapon::OnReload()
{
	

	if (Ammo >= 30 || AmmoPool <= 0)
	{
		return;
	}

	if (AmmoPool < (30 - Ammo))
	{
		Ammo = Ammo + AmmoPool;
		AmmoPool = AmmoPool - 30;
	}
	else
	{
		AmmoPool = AmmoPool - (30 - Ammo);
		Ammo = 30;
		
	}
	
}



void AARBaseWeapon::PlayFireEffects(FVector TraceEnd)
{
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
	}

	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetOwner()->GetActorLocation());
		
	}
	

	APawn* MyOwner = Cast<APawn>(GetOwner());
	if (MyOwner)
	{
		APlayerController* PC = Cast<APlayerController>(MyOwner->GetController());
		if (PC)
		{
			PC->ClientPlayCameraShake(FireCamShake);
		}
	}
}

void AARBaseWeapon::PlayImpactEffects(EPhysicalSurface SurfaceType, FVector ImpactPoint)
{
	UParticleSystem* SelectedEffect = nullptr;
	switch (SurfaceType)
	{
	case SURFACE_PLAYERARMORED:
		SelectedEffect = PlayerImpactEffect;
		break;
	case SURFACE_PLAYERUNARMORED:
		SelectedEffect = PlayerImpactEffect;
		break;
	default:
		SelectedEffect = DefaultImpactEffect;
		break;
	}

	if (SelectedEffect)
	{
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);

		FVector ShotDirection = ImpactPoint - MuzzleLocation;
		ShotDirection.Normalize();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, ImpactPoint);
	}
}

void AARBaseWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const

{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AARBaseWeapon, HitScanTrace, COND_SkipOwner);
}
