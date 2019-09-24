// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerRow.h"

#include "Components/Button.h"

#include "MenuSystem/MainMenu.h"

void UServerRow::Setup(UMainMenu* InParent, uint32 InIndex)
{
	Parent = InParent;
	Index = InIndex;
	RowBtn->OnClicked.AddDynamic(this, &UServerRow::OnClicked);
}

void UServerRow::OnClicked()
{
	Parent->SelectIndex(Index);
}
