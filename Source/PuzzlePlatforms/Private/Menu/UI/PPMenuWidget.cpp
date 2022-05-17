// Fill out your copyright notice in the Description page of Project Settings.

#include "Menu/UI/PPMenuWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Menu/UI/PPServerRowWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogPPMenuWidget, All, All);

UPPMenuWidget::UPPMenuWidget(const FObjectInitializer& ObjectInitializer) {
    ConstructorHelpers::FClassFinder<UPPServerRowWidget> ServerRowBPClass(TEXT("/Game/Menu/UI/WBP_ServerRowWidget"));
    if (ServerRowBPClass.Class)
    {
        ServerRowWidgetClass = ServerRowBPClass.Class;
    }
}

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
    
    if (RefreshServerListButton)
    {
        RefreshServerListButton->OnClicked.AddDynamic(this, &UPPMenuWidget::OnRefreshServerList);
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

    Teardown();
    MenuInterface->JoinGame(DefaultIPAddress);
}

void UPPMenuWidget::OnShowJoinBanner()
{
    if (!MenuSwitcher || !JoinBanner || !MenuInterface) return;

    MenuInterface->RefreshServerList();
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

void UPPMenuWidget::OnRefreshServerList()
{
    if (!MenuInterface) return;
    
    MenuInterface->RefreshServerList();
}

void UPPMenuWidget::SetServerList(TArray<FString> ServerNames)
{
    if (!GetWorld() || !ServerListScrollBox || !ServerRowWidgetClass) return;

    ServerListScrollBox->ClearChildren();
    for (const auto& ServerName : ServerNames)
    {
        auto ServerRowWidget = CreateWidget<UPPServerRowWidget>(GetWorld(), ServerRowWidgetClass);
        if (!ServerRowWidget) return;

        ServerListScrollBox->AddChild(ServerRowWidget);

        ServerRowWidget->SetServerName(FText::FromString(ServerName));
    }
}
