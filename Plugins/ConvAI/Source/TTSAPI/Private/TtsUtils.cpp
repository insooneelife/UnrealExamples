#include "TtsUtils.h"
#include "Http.h"
#include "JsonObjectConverter.h"

TSharedRef<IHttpRequest> FTtsUtils::CreateRequest(const FTTSRequestBody &BodyData)
{
    const FString GET = TEXT("GET");
    const FString POST = TEXT("POST");
    const FString ContentType = TEXT("Content-Type");
    const FString ContentTypeJson = TEXT("application/json");

    FHttpModule *Http = &FHttpModule::Get();
    const FString URL = TEXT("http://localhost:5000/tts");
    const FString Verb = GET;

    FString OutJsonString;
    FJsonObjectConverter::UStructToJsonObjectString(BodyData, OutJsonString);

    TSharedRef<IHttpRequest> Request = Http->CreateRequest();
    Request->SetURL(URL);
    Request->SetVerb(Verb);
    Request->SetHeader(ContentType, ContentTypeJson);
    Request->SetContentAsString(OutJsonString);

    return Request;
}