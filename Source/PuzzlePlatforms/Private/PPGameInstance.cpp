// Fill out your copyright notice in the Description page of Project Settings.

#include "PPGameInstance.h"
#include "Engine/Engine.h"

void UPPGameInstance::Host()
{
    if (!GetEngine()) return;

    GetEngine()->AddOnScreenDebugMessage(0, 5, FColor::Red, TEXT("Hosting"));

    if (!GetWorld()) return;

    GetWorld()->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}

void UPPGameInstance::Join(FString Address)
{
    if (!GetEngine()) return;

    GetEngine()->AddOnScreenDebugMessage(0, 5, FColor::Red, FString::Printf(TEXT("Joining %s"), *Address));

    const auto LocalPlayerController = GetFirstLocalPlayerController();
    if (!LocalPlayerController) return;

    LocalPlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}
