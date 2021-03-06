// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PPMenuInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPPMenuInterface : public UInterface
{
	GENERATED_BODY()
};

class PUZZLEPLATFORMS_API IPPMenuInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    virtual void HostGame(const FString& ServerName) = 0;

    virtual void JoinGame(uint32 ServerIndex) = 0;

    virtual void LoadMainMenu() = 0;

    virtual void RefreshServerList() = 0;
};
