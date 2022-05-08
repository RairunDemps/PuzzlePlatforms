// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Platforms/PPBaseMovingPlatform.h"
#include "PPTriggerMovingPlatform.generated.h"

UCLASS()
class PUZZLEPLATFORMS_API APPTriggerMovingPlatform : public APPBaseMovingPlatform
{
	GENERATED_BODY()

public:
    void AddActiveTrigger();
    void RemoveActiveTrigger();

protected:
    virtual void Tick(float DeltaSeconds) override;

private:
    int32 ActiveTriggers = 0;
};
