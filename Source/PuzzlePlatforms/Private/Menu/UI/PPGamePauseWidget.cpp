// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu/UI/PPGamePauseWidget.h"
#include "Components/Button.h"

void UPPGamePauseWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (ResumeButton)
    {
        ResumeButton->OnClicked.AddDynamic(this, &UPPGamePauseWidget::OnResumeGame);
    }

    if (QuitButton)
    {
        QuitButton->OnClicked.AddDynamic(this, &UPPGamePauseWidget::OnQuitGame);
    }
}

void UPPGamePauseWidget::OnResumeGame()
{
    Teardown();
}

void UPPGamePauseWidget::OnQuitGame()
{
    if (!MenuInterface) return;

    MenuInterface->LoadMainMenu();
}
