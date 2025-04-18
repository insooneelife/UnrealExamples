#include "ChatGPTUtils.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Http.h"
#include "JsonObjectConverter.h"
#include "Json.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "HAL/PlatformMisc.h"



TSharedRef<IHttpRequest> FChatGPTUtils::CreateRequest(const FString& APIKey, const FString& Prompt)
{
	FChatGPTRequestBody RequestBody;
	FChatMessage Message;
	Message.role = TEXT("user");
	Message.content = Prompt;
	RequestBody.messages.Add(Message);

	FString JsonString;
	FJsonObjectConverter::UStructToJsonObjectString(RequestBody, JsonString);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *APIKey));
	Request->SetURL(TEXT("https://api.openai.com/v1/chat/completions"));
	Request->SetContentAsString(JsonString);

	return Request;
}

bool FChatGPTUtils::ParseResponse(TSharedPtr<IHttpResponse, ESPMode::ThreadSafe> Response, FChatGPTResponse& OutResponse)
{
	const FString ResponseString = Response->GetContentAsString();
	if (FJsonObjectConverter::JsonObjectStringToUStruct<FChatGPTResponse>(ResponseString, &OutResponse, 0, 0))
	{
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("chat gpt response parse failed. ResponseString : %s"), *ResponseString);
		return false;
	}
}


FString FChatGPTUtils::GetAPIKey()
{
	// Windows API GetEnvironmentVariable
	TCHAR Buffer[1024];
	FPlatformMisc::GetEnvironmentVariable(TEXT("OPENAI_API_KEY"), Buffer, 1024);
	FString APIKey = FString(Buffer);

	UE_LOG(LogTemp, Log, TEXT("APIKey : %s"), *APIKey);
	return APIKey;
}