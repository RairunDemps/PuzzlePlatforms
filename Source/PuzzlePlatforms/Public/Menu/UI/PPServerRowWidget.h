// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PPServerRowWidget.generated.h"

class UTextBlock;
class UButton;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnServerSelectedSignature, uint32);

UCLASS()
class PUZZLEPLATFORMS_API UPPServerRowWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    FOnServerSelectedSignature OnServerSelected;

	void SetServerName(const FText& ServerName);
    FText GetServerName() const;

    void SetServerIndex(uint32 Index);
    uint32 GetServerIndex() const { return ServerIndex; }

    void SetSelected(bool IsSelected);

protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ServerNameTextBlock;

	UPROPERTY(meta = (BindWidget))
    UButton* SelectServerButton;

	void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnSelectServerButtonClicked();

    UFUNCTION()
	void OnSelectServerButtonHovered();

    UFUNCTION()
    void OnSelectServerButtonUnhovered();

	uint32 ServerIndex;
    bool IsServerSelected;
};
