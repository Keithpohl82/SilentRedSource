// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuSystem/MenuWidget.h"
#include "InGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class SILENTRED2_API UInGameMenu : public UMenuWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize();


private:
	UPROPERTY(meta = (BindWidget))
		class UButton* InGameMainMenuBtn;

	UPROPERTY(meta = (BindWidget))
		class UButton* InGameCancelBtn;

	UFUNCTION()
		void CancelPressed();

	UFUNCTION()
		void QuitPressed();
};
