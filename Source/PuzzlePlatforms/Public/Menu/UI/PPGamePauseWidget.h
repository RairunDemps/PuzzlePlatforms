// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Menu/UI/PPBaseWidget.h"
#include "PPGamePauseWidget.generated.h"

class UButton;

UCLASS()
class PUZZLEPLATFORMS_API UPPGamePauseWidget : public UPPBaseWidget
{
	GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* ResumeButton;

    UPROPERTY(meta = (BindWidget))
    UButton* QuitButton;

    void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnResumeGame();

    UFUNCTION()
    void OnQuitGame();
};
