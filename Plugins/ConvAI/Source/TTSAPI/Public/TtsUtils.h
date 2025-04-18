#pragma once

#include "CoreMinimal.h"
#include "TtsUtils.generated.h"

class IHttpRequest;

USTRUCT(BlueprintType)
struct FTTSRequestBody
{
    GENERATED_USTRUCT_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Data;
};

class TTSAPI_API FTtsUtils
{
public:
    static TSharedRef<IHttpRequest> CreateRequest(const FTTSRequestBody &BodyData);
};
