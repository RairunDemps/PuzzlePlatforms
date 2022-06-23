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
    IsShowDebugScreenMessage = false;
}

void APPBaseMovingPlatform::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (HasAuthority())
    {
        ReplicatedCurrentTripTime = CurrentTripTime;
    }
    
    if (IsShowDebugScreenMessage)
    {
        FString RoleString;
        UEnum::GetValueAsString(GetLocalRole(), RoleString);
        DrawDebugString(GetWorld(), FVector(0.0f, 0.0f, 100.0f), RoleString, this, FColor::White, DeltaSeconds);
    }
}

void APPBaseMovingPlatform::Move(float DeltaSeconds)
{
    const float Frequency = (2 * PI) / FullTripTime;
    FVector CurrentLocation = GetActorLocation();

    CurrentTripTime += DeltaSeconds;

    CurrentLocation.X = StartLocation.X + (DestinationLocation.X - StartLocation.X) *
                                                FMath::Abs(FMath::Sin((Frequency * CurrentTripTime) - PI / 2.0f) + 1) / 2.0f;
    CurrentLocation.Y = StartLocation.Y + (DestinationLocation.Y - StartLocation.Y) *
                                                FMath::Abs(FMath::Sin((Frequency * CurrentTripTime) - PI / 2.0f) + 1) / 2.0f;
    CurrentLocation.Z = StartLocation.Z + (DestinationLocation.Z - StartLocation.Z) *
                                                FMath::Abs(FMath::Sin((Frequency * CurrentTripTime) - PI / 2.0f) + 1) / 2.0f;

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

void APPBaseMovingPlatform::ShowDebugScreenMessages()
{
    IsShowDebugScreenMessage = true;
}

void APPBaseMovingPlatform::HideDebugScreenMessages()
{
    IsShowDebugScreenMessage = false;
}
