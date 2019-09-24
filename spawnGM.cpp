// Fill out your copyright notice in the Description page of Project Settings.


#include "spawnGM.h"
#include "TestPlayerState.h"

#include "Engine/World.h"



AspawnGM::AspawnGM()
{
	PlayerStateClass = ATestPlayerState::StaticClass();
	
}



void AspawnGM::RespawnDeadPlayers()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator();  It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC && PC->GetPawn() == nullptr)
		{
			RestartPlayer(PC);
		}
	}
}


