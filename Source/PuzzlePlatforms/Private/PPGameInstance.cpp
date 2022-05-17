// Fill out your copyright notice in the Description page of Project Settings.

#include "PPGameInstance.h"
#include "GameFramework/GameModeBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Menu/UI/PPMenuWidget.h"
#include "Menu/UI/PPGamePauseWidget.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

const static FName SESSION_NAME = TEXT("My session name");

DEFINE_LOG_CATEGORY_STATIC(LogPPGameInstance, All, All);

UPPGameInstance::UPPGameInstance(const FObjectInitializer& ObjectInitializer)
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

    SessionSearch = MakeShareable(new FOnlineSessionSearch());
    if (!SessionSearch.IsValid()) return;

    SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPPGameInstance::OnDestroySessionComplete);
    SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPPGameInstance::OnCreateSessionComplete);
    SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPPGameInstance::OnFindSessionsComplete);
    SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPPGameInstance::OnJoinSessionComplete);
}

void UPPGameInstance::HostGame()
{
    if (!SessionInterface.IsValid()) return;

    const auto NamedSession = SessionInterface->GetNamedSession(SESSION_NAME);
    if (NamedSession)
    {
        UE_LOG(LogPPGameInstance, Display, TEXT("Session \"%s\" was found."), *SESSION_NAME.ToString());
        SessionInterface->DestroySession(SESSION_NAME);
        return;
    }

    CreateSession();
}

void UPPGameInstance::CreateSession()
{
    if (!SessionInterface.IsValid()) return;

    UE_LOG(LogPPGameInstance, Display, TEXT("Creating a session \"%s\"."), *SESSION_NAME.ToString());
    FOnlineSessionSettings SessionSettings;
    SessionSettings.bIsLANMatch = true;
    SessionSettings.bShouldAdvertise = true;
    SessionSettings.NumPublicConnections = 2;
    SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
}

void UPPGameInstance::JoinGame(uint32 ServerIndex)
{
    if (!SessionInterface.IsValid() || !SessionSearch.IsValid()) return;

    SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[ServerIndex]);
}

void UPPGameInstance::LoadMainMenu()
{
    if (!GetWorld()) return;

    const auto Controller = GetWorld()->GetFirstPlayerController();
    if (!Controller) return;

    Controller->ClientTravel("/Game/Menu/MenuLevel", ETravelType::TRAVEL_Absolute);
}

void UPPGameInstance::RefreshServerList()
{
    if (!SessionInterface.IsValid() || !SessionSearch.IsValid()) return;

    SessionSearch->bIsLanQuery = true;
    SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
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
    if (!SessionSearch.IsValid() || !IsSuccessful) return;

    TArray<FString> ServerNames;

    for (const auto& SearchResult : SessionSearch->SearchResults)
    {
        if (!SearchResult.IsValid()) continue;
        
        ServerNames.Add(SearchResult.GetSessionIdStr());
    }

    if (!MenuWidget) return;
    MenuWidget->SetServerList(ServerNames);
}

void UPPGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    if (!GetWorld() || !SessionInterface.IsValid() || Result != EOnJoinSessionCompleteResult::Success) return;

    FString TravelURL;
    const bool IsGotConnectionString = SessionInterface->GetResolvedConnectString(SESSION_NAME, TravelURL);
    if (!IsGotConnectionString) return;

    UE_LOG(LogPPGameInstance, Display, TEXT("%s"), *TravelURL);

    const auto Controller = GetWorld()->GetFirstPlayerController();
    if (!Controller) return;

    Controller->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
}
