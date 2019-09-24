// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHealthComponent.h"
#include "spawnGM.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UPlayerHealthComponent::UPlayerHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	DefaultHealth = 100;
	DefaultArmor = 100;
	bIsDead = false;

	SetIsReplicated(true);
}


// Called when the game starts
void UPlayerHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwnerRole() == ROLE_Authority)
	{
		AActor* MyOwner = GetOwner();
		if (MyOwner)
		{
			MyOwner->OnTakeAnyDamage.AddDynamic(this, &UPlayerHealthComponent::HandleTakeAnyDamage);
		}
	}
	
	Health = DefaultHealth;
	Armor = DefaultArmor;
}

void UPlayerHealthComponent::HandleTakeAnyDamage(AActor* DamageActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || bIsDead)
	{
		return;
	}
	
	Armor = (Armor - Damage);
	if (Armor <= 0)
	{
		
		Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);
	}
	
	
	UE_LOG(LogTemp, Log, TEXT("Health Changed: %s"), *FString::SanitizeFloat(Health));

	bIsDead = Health <= 0.0f;

	OnHealthChanged.Broadcast(this, Armor, Health, Damage, DamageType, InstigatedBy, DamageCauser);

	if (bIsDead)
	{
		AspawnGM* GM = Cast<AspawnGM>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->OnActorKilled.Broadcast(GetOwner(), DamageCauser, InstigatedBy);
		}
	}
}

void UPlayerHealthComponent::AddHealth(float HealAmount)
{

	if (HealAmount <= 0.0f || Health <= 0.0f)
	{
		return;
	}
	Health = FMath::Clamp(Health + HealAmount, 0.0f, DefaultHealth);

}

void UPlayerHealthComponent::AddArmor(float ArmorAdded)
{
	if (ArmorAdded <= 0.0f || Health <= 0.0f)
	{
		return;
	}
	Armor = FMath::Clamp(Armor + ArmorAdded, 0.0f, DefaultArmor);
}

void UPlayerHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const

{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerHealthComponent, Health);
	DOREPLIFETIME(UPlayerHealthComponent, Armor);
	

}