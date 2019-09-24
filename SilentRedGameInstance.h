// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MainMenuInterface.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionInterface.h"


#include "SilentRedGameInstance.generated.h"


/**
 * 
 */
UCLASS()
class SILENTRED2_API USilentRedGameInstance : public UGameInstance, public IMainMenuInterface
{
	GENERATED_BODY()
		
	public:

		USilentRedGameInstance(const FObjectInitializer & ObjectInitializer);

		virtual void Init();

		

		UFUNCTION(BlueprintCallable)
		void LoadMenuWidget();

		UFUNCTION(BlueprintCallable)
		void InGameLoadMenu();

		UFUNCTION(Exec)
		void Host(FString ServerName) override;

		UFUNCTION(Exec)
		void Join(uint32 Index) override;

		void RefreshServerList() override;

		virtual void LoadMainMenu() override;

		
	

private:

	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> InGameMenuClass;

	class UMainMenu* Menu;


	IOnlineSessionPtr SessionInterface;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);


	FString DesiredServerName;

	void CreateSession();

	
};
