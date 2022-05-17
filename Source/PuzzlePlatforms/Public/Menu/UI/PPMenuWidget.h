// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Menu/UI/PPBaseWidget.h"
#include "PPMenuWidget.generated.h"

class UButton;
class UWidget;
class UWidgetSwitcher;
class UEditableTextBox;
class UScrollBox;
class UPPServerRowWidget;

UCLASS()
class PUZZLEPLATFORMS_API UPPMenuWidget : public UPPBaseWidget
{
    GENERATED_BODY()

public:
    UPPMenuWidget(const FObjectInitializer& ObjectInitializer);

    void Setup() override;
    void SetServerList(TArray<FString> ServerNames);

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* HostButton;

    UPROPERTY(meta = (BindWidget))
    UButton* JoinButton;
    
    UPROPERTY(meta = (BindWidget))
    UWidgetSwitcher* MenuSwitcher;

    UPROPERTY(meta = (BindWidget))
    UWidget* MenuBanner;

    UPROPERTY(meta = (BindWidget))
    UWidget* JoinBanner;

    UPROPERTY(meta = (BindWidget))
    UButton* GoButton;

    UPROPERTY(meta = (BindWidget))
    UButton* CancelButton;

    UPROPERTY(meta = (BindWidget))
    UButton* ExitButton;

    UPROPERTY(meta = (BindWidget))
    UButton* RefreshServerListButton;

    UPROPERTY(meta = (BindWidget))
    UScrollBox* ServerListScrollBox;

    void NativeOnInitialized() override;

private:
    FString DefaultIPAddress = FString("127.0.0.1");

    UFUNCTION()
    void OnHostGame();

    UFUNCTION()
    void OnShowJoinBanner();

    UFUNCTION()
    void OnShowMenuBanner();

    UFUNCTION()
    void OnJoinGame();

    UFUNCTION()
    void OnExitGame();
    
    UFUNCTION()
    void OnRefreshServerList();

    TSubclassOf<UPPServerRowWidget> ServerRowWidgetClass;
};
