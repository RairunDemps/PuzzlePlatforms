// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Platforms/PPBaseMovingPlatform.h"
#include "PPMovingPlatformStaticMeshActor.generated.h"

UCLASS()
class PUZZLEPLATFORMS_API APPMovingPlatformStaticMeshActor : public APPBaseMovingPlatform
{
	GENERATED_BODY()

protected:
    virtual void Tick(float DeltaSeconds) override;
};
