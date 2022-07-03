// Fill out your copyright notice in the Description page of Project Settings.


#include "Platforms/PPBaseMovingPlatform.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"

APPBaseMovingPlatform::APPBaseMovingPlatform()
{
    PrimaryActorTick.bCanEverTick = true;

    bReplicates = true;
    SetReplicatingMovement(false);
    SetMobility(EComponentMobility::Movable);
}

void APPBaseMovingPlatform::BeginPlay()
{
    Super::BeginPlay();

    StartLocation = GetActorLocation();
    CurrentTripTime = 0.0f;
}

void APPBaseMovingPlatform::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (HasAuthority())
    {
        ReplicatedCurrentTripTime = CurrentTripTime;
    }
}

void APPBaseMovingPlatform::Move(float DeltaSeconds)
{
    const float Frequency = (2 * PI) / FullTripTime;
    FVector CurrentLocation = GetActorLocation();

    CurrentTripTime += DeltaSeconds;

    float LocationCoefficient = (1 - FMath::Cos(Frequency * CurrentTripTime)) / 2.0f;
    CurrentLocation.X = StartLocation.X + (DestinationLocation.X - StartLocation.X) * LocationCoefficient;
    CurrentLocation.Y = StartLocation.Y + (DestinationLocation.Y - StartLocation.Y) * LocationCoefficient;
    CurrentLocation.Z = StartLocation.Z + (DestinationLocation.Z - StartLocation.Z) * LocationCoefficient;

    SetActorLocation(CurrentLocation);
}

void APPBaseMovingPlatform::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(APPBaseMovingPlatform, ReplicatedCurrentTripTime);
}

void APPBaseMovingPlatform::OnRep_CurrentTripTime()
{
    CurrentTripTime = ReplicatedCurrentTripTime;
}
