// Fill out your copyright notice in the Description page of Project Settings.

#include "Menu/UI/PPMenuWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"

void UPPMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (HostButton)
    {
        HostButton->OnClicked.AddDynamic(this, &UPPMenuWidget::OnHostGame);
    }

    if (JoinButton)
    {
        JoinButton->OnClicked.AddDynamic(this, &UPPMenuWidget::OnShowJoinBanner);
    }

    if (CancelButton)
    {
        CancelButton->OnClicked.AddDynamic(this, &UPPMenuWidget::OnShowMenuBanner);
    }

    if (GoButton)
    {
        GoButton->OnClicked.AddDynamic(this, &UPPMenuWidget::OnJoinGame);
    }

    if (ExitButton)
    {
        ExitButton->OnClicked.AddDynamic(this, &UPPMenuWidget::OnExitGame);
    }
}

void UPPMenuWidget::Setup()
{
    Super::Setup();

    OnShowMenuBanner();
}

void UPPMenuWidget::OnHostGame()
{
    if (!MenuInterface) return;

    Teardown();
    MenuInterface->HostGame();
}

void UPPMenuWidget::OnJoinGame()
{
    if (!MenuInterface) return;

    const FText& IPAddress = IPAddressEditableTextBox->GetText();
    const FString& JoinIPAddress = IPAddress.IsEmpty() ? DefaultIPAddress : IPAddress.ToString();
    
    Teardown();
    MenuInterface->JoinGame(JoinIPAddress);
}

void UPPMenuWidget::OnShowJoinBanner()
{
    if (!MenuSwitcher || !JoinBanner) return;

    MenuSwitcher->SetActiveWidget(JoinBanner);
}

void UPPMenuWidget::OnShowMenuBanner()
{
    if (!MenuSwitcher || !MenuBanner) return;

    MenuSwitcher->SetActiveWidget(MenuBanner);
}

void UPPMenuWidget::OnExitGame()
{
    const auto Controller = GetPlayerController();
    if (!Controller) return;

    Controller->ConsoleCommand("quit");
}
