// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"
#include "Components/Button.h"

bool UInGameMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(InGameCancelBtn != nullptr)) return false;
	InGameCancelBtn->OnClicked.AddDynamic(this, &UInGameMenu::CancelPressed);

	if (!ensure(InGameMainMenuBtn != nullptr)) return false;
	InGameMainMenuBtn->OnClicked.AddDynamic(this, &UInGameMenu::QuitPressed);

	return true;
}

void UInGameMenu::CancelPressed()
{
	Teardown();
}

void UInGameMenu::QuitPressed()
{
	if (MainMenuInterface != nullptr)
	{
		Teardown();
		MainMenuInterface->LoadMainMenu();
	}
}
