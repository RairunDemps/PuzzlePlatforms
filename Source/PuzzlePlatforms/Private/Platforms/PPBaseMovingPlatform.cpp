// Fill out your copyright notice in the Description page of Project Settings.


#include "Platforms/PPBaseMovingPlatform.h"

APPBaseMovingPlatform::APPBaseMovingPlatform()
{
    PrimaryActorTick.bCanEverTick = true;

    SetMobility(EComponentMobility::Movable);
}

void APPBaseMovingPlatform::BeginPlay()
{
    Super::BeginPlay();

    if (GetNetMode() == NM_ListenServer)
    {
        IsAuthorative = true;
        SetReplicates(true);
        SetReplicateMovement(true);
    }

    StartLocation = GetActorLocation();
}

void APPBaseMovingPlatform::Move(float DeltaSeconds)
{
    FVector CurrentLocation = GetActorLocation();
    float Frequency = (2 * PI) / FullTripTime;

    CurrentTripTime += DeltaSeconds;

    CurrentLocation.X = StartLocation.X + (DestinationLocation.X - StartLocation.X) *
            FMath::Abs(FMath::Sin((Frequency * CurrentTripTime) - PI / 2.0f) + 1) / 2.0f;
    CurrentLocation.Y = StartLocation.Y + (DestinationLocation.Y - StartLocation.Y) *
            FMath::Abs(FMath::Sin((Frequency * CurrentTripTime) - PI / 2.0f) + 1) / 2.0f;
    CurrentLocation.Z = StartLocation.Z + (DestinationLocation.Z - StartLocation.Z) *
            FMath::Abs(FMath::Sin((Frequency * CurrentTripTime) - PI / 2.0f) + 1) / 2.0f;

    SetActorLocation(CurrentLocation);
}
