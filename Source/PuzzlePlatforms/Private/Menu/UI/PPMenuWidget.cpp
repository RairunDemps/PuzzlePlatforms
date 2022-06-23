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
        HostButton->OnClicked.AddDynamic(this, &UPPMenuWidget::OnShowHostBanner);
    }

    if (JoinButton)
    {
        JoinButton->OnClicked.AddDynamic(this, &UPPMenuWidget::OnShowJoinBanner);
    }

    if (JoinCancelButton)
    {
        JoinCancelButton->OnClicked.AddDynamic(this, &UPPMenuWidget::OnShowMenuBanner);
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
    
    if (HostCancelButton)
    {
        HostCancelButton->OnClicked.AddDynamic(this, &UPPMenuWidget::OnShowMenuBanner);
    }

    if (CreateButton)
    {
        CreateButton->OnClicked.AddDynamic(this, &UPPMenuWidget::OnHostGame);
    }
}

void UPPMenuWidget::Setup()
{
    Super::Setup();

    OnShowMenuBanner();
}

void UPPMenuWidget::OnHostGame()
{
    if (!MenuInterface || !ServerNameTextBox) return;

    Teardown();
    MenuInterface->HostGame(ServerNameTextBox->GetText().ToString());
}

void UPPMenuWidget::OnJoinGame()
{
    if (!MenuInterface || !SelectedIndex.IsSet()) return;

    Teardown();
    MenuInterface->JoinGame(SelectedIndex.GetValue());
}

void UPPMenuWidget::OnShowMenuBanner()
{
    if (!MenuSwitcher || !MenuBanner) return;

    MenuSwitcher->SetActiveWidget(MenuBanner);
}

void UPPMenuWidget::OnShowJoinBanner()
{
    if (!MenuSwitcher || !JoinBanner || !MenuInterface) return;

    MenuInterface->RefreshServerList();
    MenuSwitcher->SetActiveWidget(JoinBanner);
}

void UPPMenuWidget::OnShowHostBanner()
{
    if (!MenuSwitcher || !HostBanner) return;

    MenuSwitcher->SetActiveWidget(HostBanner);
}

void UPPMenuWidget::OnExitGame()
{
    APlayerController* const Controller = GetPlayerController();
    if (!Controller) return;

    Controller->ConsoleCommand("quit");
}

void UPPMenuWidget::OnRefreshServerList()
{
    if (!MenuInterface) return;
    
    ServerListScrollBox->ClearChildren();
    MenuInterface->RefreshServerList();
}

void UPPMenuWidget::SetServerList(TArray<FServerData> ServerData)
{
    if (!GetWorld() || !ServerListScrollBox || !ServerRowWidgetClass) return;

    ServerListScrollBox->ClearChildren();
    uint32 index = 0;
    for (const FServerData& OneServerData : ServerData)
    {
        UPPServerRowWidget* const ServerRowWidget = CreateWidget<UPPServerRowWidget>(GetWorld(), ServerRowWidgetClass);
        if (!ServerRowWidget) return;
        
        ServerRowWidget->SetServerName(FText::FromString(OneServerData.Name));
        ServerRowWidget->SetHostUsername(FText::FromString(OneServerData.HostUsername));
        ServerRowWidget->SetConnectionFraction(OneServerData.CurrentPlayesCount, OneServerData.MaximumPlayerNumber);
        ServerRowWidget->SetServerIndex(index);
        ServerRowWidget->OnServerSelected.AddUObject(this, &UPPMenuWidget::OnServerSelected);
        ServerListScrollBox->AddChild(ServerRowWidget);
        ++index;
    }
}

void UPPMenuWidget::OnServerSelected(uint32 Index)
{
    SelectedIndex = Index;
    for (int i = 0; i < ServerListScrollBox->GetChildrenCount(); ++i)
    {
        UPPServerRowWidget* const ServerRowWidget = Cast<UPPServerRowWidget>(ServerListScrollBox->GetChildAt(i));
        if (!ServerRowWidget) continue;

        const bool IsSelected = ServerRowWidget->GetServerIndex() == Index;
        ServerRowWidget->SetSelected(IsSelected);
    }
}

