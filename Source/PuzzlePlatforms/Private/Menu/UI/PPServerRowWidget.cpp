// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu/UI/PPServerRowWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Menu/UI/PPMenuWidget.h"

void UPPServerRowWidget::SetServerName(const FText& ServerName)
{
	if (!ServerNameTextBlock) return;

	ServerNameTextBlock->SetText(ServerName);
}

void UPPServerRowWidget::Setup(UPPMenuWidget* Parent, uint32 Index)
{
    if (!Parent) return;

    MenuWidget = Parent;
    ServerIndex = Index;

    if (SelectServerButton)
    {
        SelectServerButton->OnClicked.AddDynamic(this, &UPPServerRowWidget::OnSelectServerButtonClicked);
    }
}

void UPPServerRowWidget::OnSelectServerButtonClicked()
{
    if (!MenuWidget) return;

    MenuWidget->SetSelectedIndex(ServerIndex);
}
