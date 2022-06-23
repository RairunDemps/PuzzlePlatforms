// Fill out your copyright notice in the Description page of Project Settings.

#include "Menu/UI/PPServerRowWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Styling/SlateColor.h"

void UPPServerRowWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (SelectServerButton)
    {
        SelectServerButton->OnClicked.AddDynamic(this, &UPPServerRowWidget::OnSelectServerButtonClicked);
    }

    if (ServerNameTextBlock)
    {
        SelectServerButton->OnHovered.AddDynamic(this, &UPPServerRowWidget::OnSelectServerButtonHovered);
        SelectServerButton->OnUnhovered.AddDynamic(this, &UPPServerRowWidget::OnSelectServerButtonUnhovered);
    }
}

void UPPServerRowWidget::SetServerName(const FText& ServerName)
{
	if (!ServerNameTextBlock) return;

	ServerNameTextBlock->SetText(ServerName);
}

FText UPPServerRowWidget::GetServerName() const
{
    return ServerNameTextBlock->GetText();
}

void UPPServerRowWidget::SetServerIndex(uint32 Index)
{
    ServerIndex = Index;
}

void UPPServerRowWidget::SetHostUsername(const FText& HostUsername)
{
    if (!HostUsernameTextBlock) return;

    HostUsernameTextBlock->SetText(HostUsername);
}

void UPPServerRowWidget::SetConnectionFraction(int32 CurrentConnectionsCount, int32 MaximumConnectionsNumber)
{
    if (!ConnectionFractionTextBlock) return;

    ConnectionFractionTextBlock->SetText(
        FText::FromString(*FString::Printf(TEXT("%d/%d"), CurrentConnectionsCount, MaximumConnectionsNumber)));
}

void UPPServerRowWidget::SetSelected(bool IsSelected)
{
    IsServerSelected = IsSelected;
    SetServerRowTextColor(FSlateColor(IsSelected ? FColor::Magenta : FColor::White));
}

void UPPServerRowWidget::OnSelectServerButtonClicked()
{
    OnServerSelected.Broadcast(ServerIndex);
}

void UPPServerRowWidget::OnSelectServerButtonHovered()
{
    if (IsServerSelected) return;

    SetServerRowTextColor(FSlateColor(FColor::Purple));
}

void UPPServerRowWidget::OnSelectServerButtonUnhovered()
{
    if (IsServerSelected) return;

    SetServerRowTextColor(FSlateColor(FLinearColor::White));
}

void UPPServerRowWidget::SetServerRowTextColor(const FSlateColor& Color)
{
    ServerNameTextBlock->SetColorAndOpacity(Color);
    HostUsernameTextBlock->SetColorAndOpacity(Color);
    ConnectionFractionTextBlock->SetColorAndOpacity(Color);
}

