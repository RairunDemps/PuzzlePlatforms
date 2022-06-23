// Fill out your copyright notice in the Description page of Project Settings.

#include "Lobby/PPLobbyGameMode.h"
#include "TimerManager.h"
#include "PPGameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogPPLobbyGameMode, All, All)

void APPLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    ++LoggedPlayersCount;
    if (LoggedPlayersCount >= MinimumRequiredPlayers)
    {
        SetStartGameTimer();
    }
}

void APPLobbyGameMode::Logout(AController* Exiting)
{
    Super::Logout(Exiting);

    --LoggedPlayersCount;
}

void APPLobbyGameMode::SetStartGameTimer()
{
    if (!GetWorld()) return;

    GetWorld()->GetTimerManager().ClearTimer(StartGameTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(StartGameTimerHandle, this, &APPLobbyGameMode::TravelToTheLevel, StartGameDelay);
}

void APPLobbyGameMode::TravelToTheLevel()
{
    if (!GetWorld()) return;

    UPPGameInstance* const PPGameInstance = GetGameInstance<UPPGameInstance>();
    if (!PPGameInstance) return;

    bUseSeamlessTravel = true;
    FString HostingString = FString("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
    PPGameInstance->StartSession();
    GetWorld()->ServerTravel(HostingString);
}
