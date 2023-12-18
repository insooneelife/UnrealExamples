// Fill out your copyright notice in the Description page of Project Settings.


#include "API/APIHandler.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "JsonObjectConverter.h"
#include "Serialization/BufferArchive.h"

const FString FAPIHandler::GET = TEXT("GET");
const FString FAPIHandler::POST = TEXT("POST");
const FString FAPIHandler::ContentType = TEXT("Content-Type");
const FString FAPIHandler::ContentTypeJson = TEXT("application/json");
const FString FAPIHandler::ContentTypeFormData = TEXT("multipart/form-data; boundary=---BOUNDARY");

// requests
void FAPIHandler::RequestSample(const FSampleRequestBody& InBodyData)
{
	FHttpModule* Http = &FHttpModule::Get();

	const FString URL = GetURL_Sample();
	const FString Verb = POST;

	FString OutJsonString;
	FJsonObjectConverter::UStructToJsonObjectString(InBodyData, OutJsonString);

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindSP(this, &FAPIHandler::OnResponseRecieved);
	Request->SetURL(URL);
	Request->SetVerb(Verb);
	Request->SetHeader(ContentType, ContentTypeJson);
	Request->SetContentAsString(OutJsonString);
	Request->ProcessRequest();

	FString FuncName(__FUNCTION__);
	LogRequest(FuncName, URL, Verb, OutJsonString);
}



void FAPIHandler::OnResponseRecieved(
	FHttpRequestPtr InRequest, FHttpResponsePtr InResponse, bool bWasSuccessful)
{
	if (InRequest == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Request is nullptr."));
		return;
	}

	if (!InRequest->DoesSharedInstanceExist())
	{
		UE_LOG(LogTemp, Error, TEXT("Request does not exists."));
		return;
	}

	if (!this->DoesSharedInstanceExist())
	{
		UE_LOG(LogTemp, Error, TEXT("this does not exists."));
		return;
	}

	FHttpModule* Http = &FHttpModule::Get();
	const FString URL = InRequest->GetURL();

	FString FuncName(__FUNCTION__);
	LogResponse(FuncName, URL, InRequest->GetVerb(), InResponse->GetResponseCode(), InResponse->GetContentAsString());

	ProcessResponses(InRequest, InResponse, bWasSuccessful);
}

void FAPIHandler::ProcessResponses(FHttpRequestPtr InRequest, FHttpResponsePtr InResponse, bool bWasSuccessful)
{
	const FString URL = InRequest->GetURL();

	if (URL == GetURL_Sample())
	{
		ResponseSample(InRequest, InResponse);
	}
}


// responses
void FAPIHandler::ResponseSample(FHttpRequestPtr InRequest, FHttpResponsePtr InResponse)
{
	if (InResponse->GetResponseCode() == Success)
	{
		FSampleResponseBody ResponseBody;
		if (FJsonObjectConverter::JsonArrayStringToUStruct(
			InResponse->GetContentAsString(), &ResponseBody.DataList))
		{
			OnResponseSample.Broadcast(ResponseBody);
		}
	}
	else
	{

	}
}



FString FAPIHandler::GetURL() const
{
	return FString(TEXT("https://jsonplaceholder.typicode.com/"));
}

FString FAPIHandler::GetURL_Sample() const
{
	return GetURL() + FString(TEXT("todos/1"));
}


void FAPIHandler::LogRequest(const FString& InFuncName, const FString& InURL, const FString& InVerb, const FString& InBody) const
{
	UE_LOG(LogTemp, Log, TEXT("%s   url : %s   verb : %s\nbody : %s"), *InFuncName, *InURL, *InVerb, *InBody);
}

void FAPIHandler::LogResponse(const FString& InFuncName, const FString& InURL, const FString& InVerb, int32 InResponseCode, const FString& InBody) const
{
	UE_LOG(LogTemp, Log, TEXT("%s   url : %s   verb : %s  responseCode : %d  \nbody : %s"),
		*InFuncName, *InURL, *InVerb, InResponseCode, *InBody);
}