// Fill out your copyright notice in the Description page of Project Settings.

#include "PPGameInstance.h"
#include "GameFramework/GameModeBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Menu/UI/PPMenuWidget.h"
#include "Menu/UI/PPGamePauseWidget.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Engine/Engine.h"

const static FName SESSION_NAME = NAME_GameSession;
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

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
    IOnlineSubsystem* const OnlineSubsystem = IOnlineSubsystem::Get();
    if (!OnlineSubsystem) return;

    UE_LOG(LogPPGameInstance, Display, TEXT("Online subsystem found - %s"), *OnlineSubsystem->GetSubsystemName().ToString())
    SessionInterface = OnlineSubsystem->GetSessionInterface();
    if (!SessionInterface.IsValid()) return;

    SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPPGameInstance::OnDestroySessionComplete);
    SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPPGameInstance::OnCreateSessionComplete);
    SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPPGameInstance::OnFindSessionsComplete);
    SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPPGameInstance::OnJoinSessionComplete);

    SessionSearch = MakeShareable(new FOnlineSessionSearch());
    if (!SessionSearch.IsValid()) return;

    if (!GEngine) return;

    GEngine->OnNetworkFailure().AddUObject(this, &UPPGameInstance::OnNetworkFailure);
}

void UPPGameInstance::HostGame(const FString& ServerName)
{
    if (!SessionInterface.IsValid()) return;

    DesiredServerName = ServerName;
    FNamedOnlineSession* const NamedSession = SessionInterface->GetNamedSession(SESSION_NAME);
    if (NamedSession)
    {
        SessionInterface->DestroySession(SESSION_NAME);
        return;
    }

    CreateSession();
}

void UPPGameInstance::CreateSession()
{
    if (!SessionInterface.IsValid()) return;

    const FName SubsystemName = IOnlineSubsystem::Get()->GetSubsystemName();
    FOnlineSessionSettings SessionSettings;
    SessionSettings.bIsLANMatch = SubsystemName.IsEqual(TEXT("NULL"), ENameCase::CaseSensitive);
    SessionSettings.bShouldAdvertise = true;
    SessionSettings.NumPublicConnections = 5;
    SessionSettings.bUsesPresence = true;
    SessionSettings.bUseLobbiesIfAvailable = true;
    SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::Type::ViaOnlineServiceAndPing);
    SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
}

void UPPGameInstance::StartSession()
{
    if (!SessionInterface.IsValid()) return;

    SessionInterface->StartSession(SESSION_NAME);
}

void UPPGameInstance::JoinGame(uint32 ServerIndex)
{
    if (!SessionInterface.IsValid() || !SessionSearch.IsValid()) return;

    SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[ServerIndex]);
}

void UPPGameInstance::LoadMainMenu()
{
    if (!GetWorld()) return;

    APlayerController* const Controller = GetWorld()->GetFirstPlayerController();
    if (!Controller) return;

    Controller->ClientTravel("/Game/Menu/MenuLevel", ETravelType::TRAVEL_Absolute);
}

void UPPGameInstance::RefreshServerList()
{
    if (!SessionInterface.IsValid() || !SessionSearch.IsValid()) return;

    const FName SubsystemName = IOnlineSubsystem::Get()->GetSubsystemName();
    SessionSearch->bIsLanQuery = SubsystemName.IsEqual(TEXT("NULL"), ENameCase::CaseSensitive);
    SessionSearch->MaxSearchResults = 100;
    SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
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
    if (GamePauseWidget)
    {
        GamePauseWidget->Setup();
        return;
    }

    if (!GamePauseWidgetClass) return;

    GamePauseWidget = CreateWidget<UPPGamePauseWidget>(this, GamePauseWidgetClass);
    if (!GamePauseWidget) return;

    GamePauseWidget->Setup();
    GamePauseWidget->SetMenuInterface(this);
}

void UPPGameInstance::OnCreateSessionComplete(FName SessionName, bool IsSuccessful)
{
    if (!GetWorld() || !IsSuccessful) return;

    const FString HostingString = FString("/Game/Maps/Lobby?listen");
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

    TArray<FServerData> ServerData;
    for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
    {
        if (!SearchResult.IsValid()) continue;

        FServerData OneServerData;
        OneServerData.MaximumPlayerNumber = SearchResult.Session.SessionSettings.NumPublicConnections;
        OneServerData.CurrentPlayesCount = OneServerData.MaximumPlayerNumber - SearchResult.Session.NumOpenPublicConnections;
        OneServerData.HostUsername = SearchResult.Session.OwningUserName;
        if (!SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, OneServerData.Name))
        {
            OneServerData.Name = TEXT("Couldn't get server name");
        }

        ServerData.Add(OneServerData);
    }

    if (!MenuWidget) return;
    MenuWidget->SetServerList(ServerData);
}

void UPPGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    if (!GetWorld() || !SessionInterface.IsValid() || Result != EOnJoinSessionCompleteResult::Success) return;

    FString TravelURL;
    const bool IsGotConnectionString = SessionInterface->GetResolvedConnectString(SESSION_NAME, TravelURL);
    if (!IsGotConnectionString) return;

    APlayerController* const Controller = GetWorld()->GetFirstPlayerController();
    if (!Controller) return;

    Controller->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
}

void UPPGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
    UE_LOG(LogTemp, Error, TEXT("Network Failure Occurred: %s"), *ErrorString);

    LoadMainMenu();
}
