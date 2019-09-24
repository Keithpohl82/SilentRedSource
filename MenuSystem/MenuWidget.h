// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuInterface.h"


#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class SILENTRED2_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:

	void SetMenuInterface(IMainMenuInterface* MainMenuInterface);

	void Setup();

	void Teardown();


protected:

	IMainMenuInterface* MainMenuInterface;
};