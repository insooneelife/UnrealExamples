#pragma once

#include "CoreMinimal.h"
#include "WhisperUtils.generated.h"

class IHttpRequest;
class IHttpResponse;

USTRUCT(BlueprintType)
struct STTAPI_API FWhisperSegment
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    int32 id;

    UPROPERTY(BlueprintReadWrite)
    int32 seek;

    UPROPERTY(BlueprintReadWrite)
    float start;

    UPROPERTY(BlueprintReadWrite)
    float end;

    UPROPERTY(BlueprintReadWrite)
    FString text;

    UPROPERTY(BlueprintReadWrite)
    TArray<int32> tokens;

    UPROPERTY(BlueprintReadWrite)
    float temperature;

    UPROPERTY(BlueprintReadWrite)
    float avg_logprob;

    UPROPERTY(BlueprintReadWrite)
    float compression_ratio;

    UPROPERTY(BlueprintReadWrite)
    float no_speech_prob;
};

USTRUCT(BlueprintType)
struct STTAPI_API FWhisperResponse
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    FString text;

    UPROPERTY(BlueprintReadWrite)
    TArray<FWhisperSegment> segments;

    UPROPERTY(BlueprintReadWrite)
    FString language;
};

class STTAPI_API FWhisperUtils
{
public:
    static TSharedRef<IHttpRequest> CreateRequest(const TArray<uint8>& AudioBuffer);
    
    static bool ParseResponse(TSharedPtr<IHttpResponse, ESPMode::ThreadSafe> Response, FWhisperResponse& OutResponse);
    
    // set string to byte array
    static void AppendStringToByteArray(const FString& InString, TArray<uint8>& OutBytes);
}; 