// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu/UI/PPServerRowWidget.h"
#include "Components/TextBlock.h"

void UPPServerRowWidget::SetServerName(const FText& ServerName)
{
	if (!ServerNameTextBlock) return;

	ServerNameTextBlock->SetText(ServerName);
}
