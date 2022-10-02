// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "APIDataTypes.h"
#include "Templates/SharedPointer.h"

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnResponseRecievedSignature, FHttpRequestPtr, FHttpResponsePtr, bool);
DECLARE_MULTICAST_DELEGATE_OneParam(FSamepleResponseSignature, const FSampleResponseBody&);

class UNREALEXAMPLES_API FAPIHandler : public TSharedFromThis<FAPIHandler>
{
public:
	const static FString GET;
	const static FString POST;
	const static FString ContentType;
	const static FString ContentTypeJson;
	const static FString ContentTypeFormData;
	const static int Success = 200;
	const static int Fail = 500;

public:

	void RequestSample(const FSampleRequestBody& InBodyData);

private:
	void OnResponseRecieved(FHttpRequestPtr InRequest, FHttpResponsePtr InResponse, bool bWasSuccessful);
	void ProcessResponses(FHttpRequestPtr InRequest, FHttpResponsePtr InResponse, bool bWasSuccessful);

	void ResponseSample(FHttpRequestPtr InRequest, FHttpResponsePtr InResponse);

public:
	FString GetURL() const;
	FString GetURL_Sample() const;

public:
	FSamepleResponseSignature OnResponseSample;

private:

	void LogRequest(const FString& InFuncName, const FString& InURL, const FString& InVerb, const FString& InBody) const;

	void LogResponse(const FString& InFuncName, const FString& InURL, const FString& InVerb, int32 InResponseCode, const FString& InBody) const;

private:
};
