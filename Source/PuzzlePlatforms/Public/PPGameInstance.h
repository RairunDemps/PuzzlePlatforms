// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Menu/PPMenuInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "PPGameInstance.generated.h"

class UPPMenuWidget;
class UPPGamePauseWidget;
class FOnlineSessionSearch;

typedef TSharedPtr<class IOnlineSession, ESPMode::ThreadSafe> IOnlineSessionPtr;

UCLASS()
class PUZZLEPLATFORMS_API UPPGameInstance : public UGameInstance, public IPPMenuInterface
{
	GENERATED_BODY()

public:
    UPPGameInstance(const FObjectInitializer& ObjectInitializer);

    void HostGame(const FString& ServerName) override;

    void JoinGame(uint32 ServerIndex) override;
    
    void LoadMainMenu() override;

    void RefreshServerList() override;

    UFUNCTION(BlueprintCallable)
    void LoadMenu();

    UFUNCTION(BlueprintCallable)
    void LoadGamePause();

    void StartSession();

	void Init() override;

private:
    TSubclassOf<UPPMenuWidget> MenuWidgetClass;
    TSubclassOf<UPPGamePauseWidget> GamePauseWidgetClass;

    UPPMenuWidget* MenuWidget;
    UPPGamePauseWidget* GamePauseWidget;

    IOnlineSessionPtr SessionInterface;
    TSharedPtr<FOnlineSessionSearch> SessionSearch;

    FString DesiredServerName;

    void CreateSession();
    void OnCreateSessionComplete(FName SessionName, bool IsSuccessful);
    void OnDestroySessionComplete(FName SessionName, bool IsSuccessful);
    void OnFindSessionsComplete(bool IsSuccessful);
    void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
};
