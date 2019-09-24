// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"

#include "MainMenu.generated.h"

USTRUCT()
struct FServerData 
{
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUserName;
	FString MapName;
};

/**
 * 
 */
UCLASS()
class SILENTRED2_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	

	UMainMenu(const FObjectInitializer & ObjectInitializer);

	void SetServerList(TArray<FServerData> ServerNames);

	void SelectIndex(uint32 Index);

protected:
	virtual bool Initialize();


private:

	TSubclassOf<class UUserWidget> ServerRowClass;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ServerList;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitGameBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* FindServerBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* RefreshBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelJoinServerBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostGameBtn;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ServerNameBox;

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelHostBtn;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;


	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* HostMenu;

	

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void OpenHostMenu();

	UFUNCTION()
	void QuitGamePressed();

	TOptional<uint32> SelectedIndex;
	
	void UpdateChildren();

};
