// Fill out your copyright notice in the Description page of Project Settings.

#include "Platforms/PPPLatformTrigger.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Platforms/PPTriggerMovingPlatform.h"

DEFINE_LOG_CATEGORY_STATIC(LogPPPlatformTrigger, All, All);

APPPLatformTrigger::APPPLatformTrigger()
{
	PrimaryActorTick.bCanEverTick = true;

	TriggerBoxComponent = CreateDefaultSubobject<UBoxComponent>("TriggerBoxComponent");
    TriggerBoxComponent->SetGenerateOverlapEvents(true);
    SetRootComponent(TriggerBoxComponent);
}

void APPPLatformTrigger::BeginPlay()
{
	Super::BeginPlay();

	check(TriggerBoxComponent);

	TriggerBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APPPLatformTrigger::OnComponentBeginOverlap);
    TriggerBoxComponent->OnComponentEndOverlap.AddDynamic(this, &APPPLatformTrigger::OnComponentEndOverlap);
}

void APPPLatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APPPLatformTrigger::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!GetWorld()) return;

    TArray<AActor*> TriggerMovingPlatforms;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), TriggerMovingPlatformClass, TriggerMovingPlatforms);
    for (AActor* const Platform : TriggerMovingPlatforms)
    {
        APPTriggerMovingPlatform* const TriggerMovingPlatform = Cast<APPTriggerMovingPlatform>(Platform);
        if (!TriggerMovingPlatform) continue;

        TriggerMovingPlatform->AddActiveTrigger();
    }
}

void APPPLatformTrigger::OnComponentEndOverlap(
    UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (!GetWorld()) return;

    TArray<AActor*> TriggerMovingPlatforms;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), TriggerMovingPlatformClass, TriggerMovingPlatforms);
    for (AActor* const Platform : TriggerMovingPlatforms)
    {
        APPTriggerMovingPlatform* const TriggerMovingPlatform = Cast<APPTriggerMovingPlatform>(Platform);
        if (!TriggerMovingPlatform) continue;

        TriggerMovingPlatform->RemoveActiveTrigger();
    }
}
