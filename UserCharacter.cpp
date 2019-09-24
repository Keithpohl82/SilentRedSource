// Fill out your copyright notice in the Description page of Project Settings.


#include "UserCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "PlayerHealthComponent.h"
#include "ARBaseWeapon.h"
#include "spawnGM.h"
#include "SilentRed2.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AUserCharacter::AUserCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);

	HealthComp = CreateDefaultSubobject<UPlayerHealthComponent>(TEXT("HealthComp"));

	WeaponAttachSocketName = "GripPoint";

	

}

// Called when the game starts or when spawned
void AUserCharacter::BeginPlay()
{
	Super::BeginPlay();

	HealthComp->OnHealthChanged.AddDynamic(this, &AUserCharacter::OnHealthChanged);

	if (Role == ROLE_Authority)
	{
		//Spawn a Default Weapon
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		CurrentWeapon = GetWorld()->SpawnActor<AARBaseWeapon>(StarterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

		if (CurrentWeapon)
		{
			CurrentWeapon->SetOwner(this);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
		}
		
	}
	
	
}

void AUserCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void AUserCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void AUserCharacter::BeginCrouch()
{
	Crouch();
}

void AUserCharacter::EndCrouch()
{
	UnCrouch();
}

void AUserCharacter::NextWeapon()
{
	
}

void AUserCharacter::PreviousWeapon()
{
}

void AUserCharacter::StartFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StartFire();
	}
}

void AUserCharacter::StopFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
}

void AUserCharacter::Reload()
{
	CurrentWeapon->OnReload();

}



void AUserCharacter::OnHealthChanged(UPlayerHealthComponent* HealthComp, float Armor, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.0f && !bDied)
	{
		//Die
		bDied = true;
		
		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		DetachFromControllerPendingDestroy();
		//Destroy();
		
		SetLifeSpan(1.0f);
		RespawnPlayer();
	}
	
	
}

void AUserCharacter::RespawnPlayer()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	SetLifeSpan(5.0f);
	AspawnGM* GM = Cast<AspawnGM>(GetWorld()->GetAuthGameMode());
	GM->RespawnDeadPlayers();
	
	
}

// Called every frame
void AUserCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AUserCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AUserCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AUserCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &AUserCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &AUserCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AUserCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AUserCharacter::EndCrouch);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AUserCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, & AUserCharacter::StopFire);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AUserCharacter::Reload);
	
	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, this, &AUserCharacter::NextWeapon);
	PlayerInputComponent->BindAction("PreviousWeapon", IE_Pressed, this, &AUserCharacter::PreviousWeapon);
}

FVector AUserCharacter::GetPawnViewLocation() const
{
	if (CameraComp)
	{
		return CameraComp->GetComponentLocation();
	}
	return Super::GetPawnViewLocation();
}

void AUserCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const

{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUserCharacter, CurrentWeapon);
	DOREPLIFETIME(AUserCharacter, bDied);
}