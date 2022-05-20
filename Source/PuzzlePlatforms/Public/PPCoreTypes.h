#pragma once

#include "PPCoreTypes.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnServerSelectedSignature, uint32)

USTRUCT()
struct FServerData
{
    GENERATED_USTRUCT_BODY()

    FString Name;
    int32 MaximumPlayerNumber;
    int32 CurrentPlayesCount;
    FString HostUsername;
};
