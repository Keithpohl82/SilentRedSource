// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"


#include "UObject/ConstructorHelpers.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

#include "ServerRow.h"

UMainMenu::UMainMenu(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/UI/WBP_ServerRow"));
	if (!ensure(ServerRowBPClass.Class != nullptr)) return;

	ServerRowClass = ServerRowBPClass.Class;
}


bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;
	
	//HostServer
	if (!ensure(HostBtn != nullptr)) return false;
	HostBtn->OnClicked.AddDynamic(this, &UMainMenu::OpenHostMenu);

	if (!ensure(FindServerBtn != nullptr)) return false;
	FindServerBtn->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(RefreshBtn != nullptr)) return false;
	RefreshBtn->OnClicked.AddDynamic(this, & UMainMenu::OpenJoinMenu);

	if (!ensure(CancelJoinServerBtn != nullptr)) return false;
	CancelJoinServerBtn->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (!ensure(JoinBtn != nullptr)) return false;
	JoinBtn->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	if (!ensure(QuitGameBtn != nullptr)) return false;
	QuitGameBtn->OnClicked.AddDynamic(this, &UMainMenu::QuitGamePressed);

	if (!ensure(HostGameBtn != nullptr)) return false;
	HostGameBtn->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!ensure(CancelHostBtn != nullptr)) return false;
	CancelHostBtn->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	return true;
	
}


void UMainMenu::HostServer()
{
	if (MainMenuInterface != nullptr)
	{
		FString ServerName = ServerNameBox->Text.ToString();
		MainMenuInterface->Host(ServerName);
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;

	MenuSwitcher->SetActiveWidget(JoinMenu);
	if (MainMenuInterface != nullptr)
	{
		MainMenuInterface->RefreshServerList();
	}
}

void UMainMenu::OpenMainMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(MainMenu != nullptr)) return;

	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::OpenHostMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(HostMenu != nullptr)) return;

	MenuSwitcher->SetActiveWidget(HostMenu);
}

void UMainMenu::SetServerList(TArray<FServerData> ServerNames)
{
	UWorld* World = this->GetWorld();
	if (!ensure(World != nullptr)) return;

	ServerList->ClearChildren();

	uint32 i = 0;
	for (const FServerData& ServerData : ServerNames)
	{
		UServerRow* Row = CreateWidget<UServerRow>(World, ServerRowClass);
		if (!ensure(Row != nullptr)) return;

		Row->ServerName->SetText(FText::FromString(ServerData.Name));
		Row->HostUserName->SetText(FText::FromString(ServerData.HostUserName));
		FString FractionText = FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayers, ServerData.MaxPlayers);
		Row->ConnectionFraction->SetText(FText::FromString(FractionText));
		Row->Setup(this, i);
		i++;

		ServerList->AddChild(Row);
	}
}

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}

void UMainMenu::UpdateChildren()
{
	for (int32 i = 0; i < ServerList->GetChildrenCount(); ++i)
	{
		auto Row = Cast<UServerRow>(ServerList->GetChildAt(i));
		if (Row != nullptr)
		{
			Row->Selected = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i);
		}
	}
}

void UMainMenu::JoinServer()
{
	if (SelectedIndex.IsSet() && MainMenuInterface != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Index %d."), SelectedIndex.GetValue());
		MainMenuInterface->Join(SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Index not set."));
	}
}

void UMainMenu::QuitGamePressed()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;


	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ConsoleCommand("quit");
}

