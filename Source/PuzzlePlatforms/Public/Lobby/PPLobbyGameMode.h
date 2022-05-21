// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuzzlePlatforms/PuzzlePlatformsGameMode.h"
#include "PPLobbyGameMode.generated.h"

UCLASS()
class PUZZLEPLATFORMS_API APPLobbyGameMode : public APuzzlePlatformsGameMode
{
	GENERATED_BODY()

public:
    void PostLogin(APlayerController* NewPlayer) override;
    void Logout(AController* Exiting) override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timer")
    float StartGameDelay = 10.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timer")
    int32 MinimumRequiredPlayers = 2;

private:
    FTimerHandle StartGameTimerHandle;

    int32 LoggedPlayersCount = 0;

    void SetStartGameTimer();
    void TravelToTheLevel();
};
