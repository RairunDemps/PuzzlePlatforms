// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Menu/PPMenuInterface.h"
#include "PPMenuWidget.generated.h"

class UButton;
class UWidget;
class UWidgetSwitcher;
class UEditableTextBox;

UCLASS()
class PUZZLEPLATFORMS_API UPPMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetMenuInterface(IPPMenuInterface* MenuInterfaceInstance);
    void Setup();

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
    UEditableTextBox* IPAddressEditableTextBox;

    void NativeOnInitialized() override;

private:
    IPPMenuInterface* MenuInterface;
    FString DefaultIPAddress = FString("127.0.0.1");

    UFUNCTION()
    void OnHostGame();

    UFUNCTION()
    void OnShowJoinBanner();

    UFUNCTION()
    void OnShowMenuBanner();

    UFUNCTION()
    void OnJoinGame();

    void Teardown();
    APlayerController* GetPlayerController() const;
};
