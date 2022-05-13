// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Menu/PPMenuInterface.h"
#include "Menu/UI/PPMenuWidget.h"
#include "PPGameInstance.generated.h"

//class AGameModeBase;

UCLASS()
class PUZZLEPLATFORMS_API UPPGameInstance : public UGameInstance, public IPPMenuInterface
{
	GENERATED_BODY()

public:
    UPPGameInstance();

    UFUNCTION(Exec)
    void HostGame();

    UFUNCTION(Exec)
    void JoinGame(const FString& Address);

    UFUNCTION(BlueprintCallable)
    void LoadMenu();

    virtual void Init() override;

private:
    TSubclassOf<UPPMenuWidget> MenuWidgetClass;

    UPPMenuWidget* MenuWidget;
};
