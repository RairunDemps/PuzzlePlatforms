// Fill out your copyright notice in the Description page of Project Settings.

#include "Menu/UI/PPBaseWidget.h"

void UPPBaseWidget::SetMenuInterface(IPPMenuInterface* MenuInterfaceInstance)
{
    check(MenuInterfaceInstance);

    MenuInterface = MenuInterfaceInstance;
}

void UPPBaseWidget::Setup()
{
    APlayerController* const Controller = GetPlayerController();
    if (!Controller) return;

    bIsFocusable = true;
    FInputModeUIOnly InputModeData;
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputModeData.SetWidgetToFocus(TakeWidget());
    Controller->SetInputMode(InputModeData);
    Controller->bShowMouseCursor = true;
    AddToViewport();
}

void UPPBaseWidget::Teardown()
{
    APlayerController* const Controller = GetPlayerController();
    if (!Controller) return;

    Controller->SetInputMode(FInputModeGameOnly());
    Controller->bShowMouseCursor = true;
    RemoveFromViewport();
}

APlayerController* UPPBaseWidget::GetPlayerController() const
{
    if (!GetWorld()) return nullptr;

    return GetWorld()->GetFirstPlayerController();
}
