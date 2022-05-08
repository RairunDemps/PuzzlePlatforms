// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPPLatformTrigger.generated.h"

class UBoxComponent;
class APPTriggerMovingPlatform;

UCLASS()
class PUZZLEPLATFORMS_API APPPLatformTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	APPPLatformTrigger();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UBoxComponent* TriggerBoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<APPTriggerMovingPlatform> TriggerMovingPlatformClass;

	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
    UFUNCTION()
    void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
    void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
