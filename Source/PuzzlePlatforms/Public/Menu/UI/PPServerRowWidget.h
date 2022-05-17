// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PPServerRowWidget.generated.h"

class UTextBlock;
class UPPMenuWidget;
class UButton;

UCLASS()
class PUZZLEPLATFORMS_API UPPServerRowWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetServerName(const FText& ServerName);
    void Setup(UPPMenuWidget* Parent, uint32 Index);

protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ServerNameTextBlock;

	UPROPERTY(meta = (BindWidget))
    UButton* SelectServerButton;

private:
    UFUNCTION()
    void OnSelectServerButtonClicked();

	UPPMenuWidget* MenuWidget;
	uint32 ServerIndex;
};
