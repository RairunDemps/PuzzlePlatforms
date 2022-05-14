// Fill out your copyright notice in the Description page of Project Settings.

#include "PPGameInstance.h"
#include "GameFramework/GameModeBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Menu/UI/PPMenuWidget.h"
#include "Menu/UI/PPGamePauseWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogPPGameInstance, All, All);

UPPGameInstance::UPPGameInstance()
{
    ConstructorHelpers::FClassFinder<UPPMenuWidget> MainMenuBPClass(TEXT("/Game/Menu/UI/WBP_MenuWidget"));
    if (MainMenuBPClass.Class)
    {
        MenuWidgetClass = MainMenuBPClass.Class;
    }

    ConstructorHelpers::FClassFinder<UPPGamePauseWidget> GamePauseBPClass(TEXT("/Game/Menu/UI/WBP_GamePauseWidget"));
    if (GamePauseBPClass.Class)
    {
        GamePauseWidgetClass = GamePauseBPClass.Class;
    }
}

void UPPGameInstance::HostGame()
{
    if (!GetWorld()) return;

    FString HostingString = FString("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");

    GetWorld()->ServerTravel(HostingString);
}

void UPPGameInstance::JoinGame(const FString& Address)
{
    if (!GetWorld()) return;

    const auto Controller = GetWorld()->GetFirstPlayerController();
    if (!Controller) return;

    Controller->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UPPGameInstance::LoadMainMenu()
{
    if (!GetWorld()) return;

    const auto Controller = GetWorld()->GetFirstPlayerController();
    if (!Controller) return;

    Controller->ClientTravel("/Game/Menu/MenuLevel", ETravelType::TRAVEL_Absolute);
}

void UPPGameInstance::LoadMenu()
{
    if (!MenuWidgetClass) return;

    MenuWidget = CreateWidget<UPPMenuWidget>(this, MenuWidgetClass);
    if (!MenuWidget) return;

    MenuWidget->Setup();

    MenuWidget->SetMenuInterface(this);
}

void UPPGameInstance::LoadGamePause()
{
    if (!GamePauseWidgetClass) return;

    GamePauseWidget = CreateWidget<UPPGamePauseWidget>(this, GamePauseWidgetClass);
    if (!GamePauseWidget) return;

    GamePauseWidget->Setup();

    GamePauseWidget->SetMenuInterface(this);
}
