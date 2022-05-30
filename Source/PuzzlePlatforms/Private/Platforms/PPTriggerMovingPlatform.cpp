// Fill out your copyright notice in the Description page of Project Settings.


#include "Platforms/PPTriggerMovingPlatform.h"

void APPTriggerMovingPlatform::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (ActiveTriggers > 0)
    {
        Move(DeltaSeconds);
    }
}

void APPTriggerMovingPlatform::AddActiveTrigger()
{
    ActiveTriggers++;
}

void APPTriggerMovingPlatform::RemoveActiveTrigger()
{
    if (ActiveTriggers > 0)
    {
        ActiveTriggers--;
    }
}