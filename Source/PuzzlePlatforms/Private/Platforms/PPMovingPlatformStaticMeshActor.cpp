// Fill out your copyright notice in the Description page of Project Settings.

#include "Platforms/PPMovingPlatformStaticMeshActor.h"

DEFINE_LOG_CATEGORY_STATIC(LogPPMovingPlatform, All, All);

void APPMovingPlatformStaticMeshActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    Move(DeltaSeconds);
}
