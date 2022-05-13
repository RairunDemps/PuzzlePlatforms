// Fill out your copyright notice in the Description page of Project Settings.

#include "Menu/UI/PPMenuWidget.h"
#include "Components/Button.h"
#include "PPGameInstance.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"

void UPPMenuWidget::NativeOnInitialized()
{
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
}

APlayerController* UPPMenuWidget::GetPlayerController() const
{
    if (!GetWorld()) return nullptr;

    return GetWorld()->GetFirstPlayerController();
}

void UPPMenuWidget::SetMenuInterface(IPPMenuInterface* MenuInterfaceInstance)
{
    check(MenuInterfaceInstance);

    MenuInterface = MenuInterfaceInstance;
}

void UPPMenuWidget::Setup()
{
    const auto Controller = GetPlayerController();
    if (!Controller) return;
    
    bIsFocusable = true;

    FInputModeUIOnly InputModeData;
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputModeData.SetWidgetToFocus(TakeWidget());

    Controller->SetInputMode(InputModeData);
    Controller->bShowMouseCursor = true;

    OnShowMenuBanner();

    AddToViewport();
}

void UPPMenuWidget::Teardown()
{
    const auto Controller = GetPlayerController();
    if (!Controller) return;

    Controller->SetInputMode(FInputModeGameOnly());
    Controller->bShowMouseCursor = true;
    RemoveFromViewport();
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
