#include "LLMAPITask.h"
#include "Http.h"
#include "Interfaces/IHttpResponse.h"
#include "JsonObjectConverter.h"


#ifndef STATGROUP_CONV_IMPL
#define STATGROUP_CONV_IMPL
DECLARE_STATS_GROUP(TEXT("CONV"), STATGROUP_CONV, STATCAT_Advanced);
#endif

DECLARE_CYCLE_STAT(TEXT("ApiLM Main"), STAT_ApiLM_Main, STATGROUP_CONV);
DECLARE_CYCLE_STAT(TEXT("ApiLM Background"), STAT_ApiLM_Background, STATGROUP_CONV);


void FLLMAPITask::StartTask()
{
	// Call parent's StartTask to change state to Progressing
	FConvTask::StartTask();

	// Start LM process
	ProcessLM();
}

bool FLLMAPITask::Tick(float DeltaTime)
{
	// Check if the task is still in progress
	if (GetTaskState() == ETaskState::Progressing)
	{
		return true;
	}

	return false;
}

void FLLMAPITask::OnTaskCompleted()
{
	// Parse ChatGPT response
	if (Response.IsValid())
	{
		//FChatGPTResponse ResponseData;
		if (FChatGPTUtils::ParseResponse(Response, ResponseData))
		{
			if (ResponseData.choices.Num() > 0)
			{
				// Broadcast completion event with response text
				LmTaskCompletedEvent.Broadcast(ResponseData.choices[0].message.content);
			}
			else
			{
				// No choices in response
				SetTaskState(ETaskState::Failed);
				OnTaskFailed(TEXT("No choices in ChatGPT response"));
			}
		}
		else
		{
			// Failed to parse response
			SetTaskState(ETaskState::Failed);
			OnTaskFailed(TEXT("Failed to parse ChatGPT response"));
		}
	}
}

void FLLMAPITask::OnTaskFailed(const FString& ErrorMessage)
{
	// Handle LM task failure
	UE_LOG(LogTemp, Error, TEXT("LM Task Failed: %s"), *ErrorMessage);
}

void FLLMAPITask::ProcessLM()
{
	SCOPE_CYCLE_COUNTER(STAT_ApiLM_Main);
	// Get API key and create request
	FString APIKey = FChatGPTUtils::GetAPIKey();
	TSharedRef<IHttpRequest> Request = FChatGPTUtils::CreateRequest(APIKey, Message);
	TSharedPtr<FLLMAPITask> This = AsShared();

	Request->OnProcessRequestComplete().BindLambda([This](FHttpRequestPtr InRequest, FHttpResponsePtr InResponse, bool bWasSuccessful)
		{
			SCOPE_CYCLE_COUNTER(STAT_ApiLM_Background);

			// Doesn't need to check if This.IsValid(), because it is captured by lambda closure
			// FLLMAPITask class cannot be destroyed as long as it is used as a TSharedPtr

			if (!bWasSuccessful || !InResponse.IsValid())
			{
				// Change state to Failed
				This->SetTaskState(ETaskState::Failed);
				// Call failure event
				This->OnTaskFailed(TEXT("Failed to get ChatGPT response"));
				return;
			}

			// Store response
			This->Response = InResponse;

			// Change state to Completed
			This->SetTaskState(ETaskState::Completed);
			// Call completion event
			This->OnTaskCompleted();
		});

	// Execute request
	Request->ProcessRequest();
}