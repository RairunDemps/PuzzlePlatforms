// Fill out your copyright notice in the Description page of Project Settings.

#include "PPGameInstance.h"
#include "GameFramework/GameModeBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Menu/UI/PPMenuWidget.h"
#include "Menu/UI/PPGamePauseWidget.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

const static FName SESSION_NAME = TEXT("My session name");

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

void UPPGameInstance::Init()
{
    const auto OnlineSubsystem = IOnlineSubsystem::Get();
    if (!OnlineSubsystem) return;

    SessionInterface = OnlineSubsystem->GetSessionInterface();
    if (!SessionInterface.IsValid()) return;

    SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPPGameInstance::OnDestroySessionComplete);
    SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPPGameInstance::OnCreateSessionComplete);
    SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPPGameInstance::OnFindSessionsComplete);

    SearchSettings = MakeShareable(new FOnlineSessionSearch());
    if (!SearchSettings.IsValid()) return;

    SessionInterface->FindSessions(0, SearchSettings.ToSharedRef());
    UE_LOG(LogPPGameInstance, Warning, TEXT("FindSessions started"));
}

void UPPGameInstance::HostGame()
{
    if (!SessionInterface.IsValid()) return;

    const auto NamedSession = SessionInterface->GetNamedSession(SESSION_NAME);
    if (NamedSession)
    {
        UE_LOG(LogPPGameInstance, Warning, TEXT("Session \"%s\" was found."), *SESSION_NAME.ToString());
        SessionInterface->DestroySession(SESSION_NAME);
        return;
    }

    CreateSession();
}

void UPPGameInstance::CreateSession()
{
    if (!SessionInterface.IsValid()) return;

    FOnlineSessionSettings SessionSettings;
    SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
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

void UPPGameInstance::OnCreateSessionComplete(FName SessionName, bool IsSuccessful)
{
    if (!GetWorld() || !IsSuccessful) return;

    FString HostingString = FString("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");

    GetWorld()->ServerTravel(HostingString);
}

void UPPGameInstance::OnDestroySessionComplete(FName SessionName, bool IsSuccessful)
{
    if (!IsSuccessful) return;

    CreateSession();
}

void UPPGameInstance::OnFindSessionsComplete(bool IsSuccessful)
{
    if (!IsSuccessful) return;

    UE_LOG(LogPPGameInstance, Warning, TEXT("FindSessions done"));
}
