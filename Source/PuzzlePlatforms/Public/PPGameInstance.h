// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Menu/PPMenuInterface.h"
#include "PPGameInstance.generated.h"

class UPPMenuWidget;
class UPPGamePauseWidget;

UCLASS()
class PUZZLEPLATFORMS_API UPPGameInstance : public UGameInstance, public IPPMenuInterface
{
	GENERATED_BODY()

public:
    UPPGameInstance();

    UFUNCTION(Exec)
    void HostGame();

    UFUNCTION(Exec)
    void JoinGame(const FString& Address);
    
    void LoadMainMenu();

    UFUNCTION(BlueprintCallable)
    void LoadMenu();

    UFUNCTION(BlueprintCallable)
    void LoadGamePause();

private:
    TSubclassOf<UPPMenuWidget> MenuWidgetClass;
    TSubclassOf<UPPGamePauseWidget> GamePauseWidgetClass;

    UPPMenuWidget* MenuWidget;
    UPPGamePauseWidget* GamePauseWidget;
};
