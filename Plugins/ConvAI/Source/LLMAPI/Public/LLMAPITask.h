#pragma once

#include "CoreMinimal.h"
#include "ConvTask.h"
#include "LLMAPIModule.h"
#include "ChatGPTUtils.h"



class IHttpRequest;
class IHttpResponse;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLmTaskCompleted, const FString&);


class LLMAPI_API FLLMAPITask : public FConvTask, public TSharedFromThis<FLLMAPITask>
{
public:
	FChatGPTResponse ResponseData;

public:
	virtual ~FLLMAPITask() { Response.Reset(); }

	virtual void StartTask() override;

	virtual bool Tick(float DeltaTime) override;

	void SetMessage(const FString& InMessage) { this->Message = InMessage; }

	FOnLmTaskCompleted& OnLmTaskCompleted() { return LmTaskCompletedEvent; }

protected:
	virtual void OnTaskCompleted() override;

	virtual void OnTaskFailed(const FString& ErrorMessage) override;

private:
	void ProcessLM();

private:
	FString Message;
	TSharedPtr<class IHttpResponse, ESPMode::ThreadSafe> Response;
	FOnLmTaskCompleted LmTaskCompletedEvent;
};