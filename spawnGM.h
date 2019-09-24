// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserCharacter.h"
#include "GameFramework/GameModeBase.h"
#include "spawnGM.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorKilled, AActor*, VictimActor, AActor*, KillerActor, AController*, KillerController);


/**
 * 
 */
UCLASS()
class SILENTRED2_API AspawnGM : public AGameModeBase
{
	GENERATED_BODY()


protected:



		

		
public:

	AspawnGM();


	UFUNCTION(Exec)
		void RespawnDeadPlayers();



	UPROPERTY(BlueprintAssignable, Category = "GameMode")
	FOnActorKilled OnActorKilled;

};
