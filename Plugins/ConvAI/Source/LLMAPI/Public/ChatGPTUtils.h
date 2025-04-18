// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChatGPTUtils.generated.h"

class IHttpRequest;
class IHttpResponse;

// request
USTRUCT(BlueprintType)
struct LLMAPI_API FChatMessage
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite, Category = "ChatGPT")
    FString role;

    UPROPERTY(BlueprintReadWrite, Category = "ChatGPT")
    FString content;
};

USTRUCT(BlueprintType)
struct LLMAPI_API FChatGPTRequestBody
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite, Category = "ChatGPT")
    TArray<FChatMessage> messages;

    UPROPERTY(BlueprintReadWrite, Category = "ChatGPT")
    FString model = TEXT("gpt-3.5-turbo");

    UPROPERTY(BlueprintReadWrite, Category = "ChatGPT")
    float temperature = 0.7f;
};


// response
USTRUCT(BlueprintType)
struct LLMAPI_API FChatGPTMessage
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	FString role;

	UPROPERTY(BlueprintReadWrite)
	FString content;
};

USTRUCT(BlueprintType)
struct LLMAPI_API FChatGPTChoice
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	FChatGPTMessage message;

	UPROPERTY(BlueprintReadWrite)
	FString finish_reason;

	UPROPERTY(BlueprintReadWrite)
	int32 index;
};

USTRUCT(BlueprintType)
struct LLMAPI_API FChatGPTUsage
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	int32 prompt_tokens;

	UPROPERTY(BlueprintReadWrite)
	int32 completion_tokens;

	UPROPERTY(BlueprintReadWrite)
	int32 total_tokens;
};

USTRUCT(BlueprintType)
struct LLMAPI_API FChatGPTResponse
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	FString id;

	UPROPERTY(BlueprintReadWrite)
	FString object;

	UPROPERTY(BlueprintReadWrite)
	int32 created;

	UPROPERTY(BlueprintReadWrite)
	FString model;

	UPROPERTY(BlueprintReadWrite)
	FChatGPTUsage usage;

	UPROPERTY(BlueprintReadWrite)
	TArray<FChatGPTChoice> choices;
};



// ChatGPT API 호출을 위한 클래스
class LLMAPI_API FChatGPTUtils
{
public:
    static TSharedRef<IHttpRequest> CreateRequest(const FString& APIKey, const FString& Message);

	static bool ParseResponse(TSharedPtr<IHttpResponse, ESPMode::ThreadSafe> Response, FChatGPTResponse& OutResponse);

	static FString GetAPIKey();

private:
}; 