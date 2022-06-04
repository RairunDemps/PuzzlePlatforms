// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PPCoreTypes.h"
#include "PPServerRowWidget.generated.h"

class UTextBlock;
class UButton;

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

    void SetHostUsername(const FText& HostUsername);
    void SetConnectionFraction(int32 CurrentConnectionsCount, int32 MaximumConnectionsNumber);

    void SetSelected(bool IsSelected);

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ServerNameTextBlock;

    UPROPERTY(meta = (BindWidget))
    UButton* SelectServerButton;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* HostUsernameTextBlock;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ConnectionFractionTextBlock;

    void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnSelectServerButtonClicked();

    UFUNCTION()
    void OnSelectServerButtonHovered();

    UFUNCTION()
    void OnSelectServerButtonUnhovered();

    void SetServerRowTextColor(const FSlateColor& Color);

    uint32 ServerIndex;
    bool IsServerSelected;
};
