// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "PPBaseMovingPlatform.generated.h"

UCLASS()
class PUZZLEPLATFORMS_API APPBaseMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()

public:
    APPBaseMovingPlatform();

    UFUNCTION(Exec)
    void ShowDebugScreenMessages();

    UFUNCTION(Exec)
    void HideDebugScreenMessages();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    FVector DestinationLocation = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    int32 FullTripTime = 10;

    void Move(float DeltaSeconds);
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

private:
    float CurrentTripTime;
    FVector StartLocation;
    bool IsShowDebugScreenMessage;

    UPROPERTY(ReplicatedUsing = OnRep_CurrentTripTime)
    float ReplicatedCurrentTripTime;

    UFUNCTION()
    void OnRep_CurrentTripTime();
};
