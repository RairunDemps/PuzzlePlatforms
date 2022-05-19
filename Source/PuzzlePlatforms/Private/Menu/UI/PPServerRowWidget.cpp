// Fill out your copyright notice in the Description page of Project Settings.

#include "Menu/UI/PPServerRowWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Styling/SlateColor.h"

void UPPServerRowWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (!SelectServerButton) return;
    SelectServerButton->OnClicked.AddDynamic(this, &UPPServerRowWidget::OnSelectServerButtonClicked);

    if (!ServerNameTextBlock) return;
    SelectServerButton->OnHovered.AddDynamic(this, &UPPServerRowWidget::OnSelectServerButtonHovered);
    SelectServerButton->OnUnhovered.AddDynamic(this, &UPPServerRowWidget::OnSelectServerButtonUnhovered);
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

void UPPServerRowWidget::SetSelected(bool IsSelected)
{
    IsServerSelected = IsSelected;
    FColor Color = IsSelected ? FColor::Magenta : FColor::White;
    ServerNameTextBlock->SetColorAndOpacity(FSlateColor(Color));
}

void UPPServerRowWidget::OnSelectServerButtonClicked()
{
    OnServerSelected.Broadcast(ServerIndex);
}

void UPPServerRowWidget::OnSelectServerButtonHovered()
{
    if (IsServerSelected) return;

    ServerNameTextBlock->SetColorAndOpacity(FSlateColor(FColor::Purple));
}

void UPPServerRowWidget::OnSelectServerButtonUnhovered()
{
    if (IsServerSelected) return;

    ServerNameTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor::White));
}
