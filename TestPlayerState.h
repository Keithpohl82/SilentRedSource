// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "TestPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SILENTRED2_API ATestPlayerState : public APlayerState
{
	GENERATED_BODY()
	

public:

	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	void AddScore(float ScoreDelta);

	ATestPlayerState();

	
};
