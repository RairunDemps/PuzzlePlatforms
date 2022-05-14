// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Menu/PPMenuInterface.h"
#include "PPBaseWidget.generated.h"

UCLASS()
class PUZZLEPLATFORMS_API UPPBaseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    virtual void SetMenuInterface(IPPMenuInterface* MenuInterfaceInstance);
    virtual void Setup();

protected:
    IPPMenuInterface* MenuInterface;

    void Teardown();
    APlayerController* GetPlayerController() const;
};
