// Fill out your copyright notice in the Description page of Project Settings.


#include "ConvAI/ConvAITaskGraphExamples.h"

#include "ConvAIModule.h"
#include "TtsUtils.h"

#include "Http.h"
#include "Misc/Paths.h"

#include "Async/Async.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundWave.h"
#include "Sound/SoundWaveProcedural.h"
#include "Audio.h"
#include "Engine/World.h"
#include "HAL/PlatformProcess.h"
#include "Async/TaskGraphInterfaces.h"


void ConvAITaskGraphExamples::AllExamples(UWorld* World)
{
	ConvAITaskGraphExample(World);
}




void ConvAITaskGraphExamples::ConvAITaskGraphExample(UWorld* World)
{
	TSharedPtr<FFlowContext> FlowContext = MakeShared<FFlowContext>();
	FGraphEventRef AudioInputTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[FlowContext]()
		{
			AudioInputTaskFunction(FlowContext->AudioInput_Result);
		}, TStatId(), nullptr, ENamedThreads::AnyThread);

	FGraphEventRef InputAudioGameTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[FlowContext, World]()
		{
			GameTaskFunction(FlowContext->AudioInput_Result.InputAudioBuffer, World);
		}, TStatId(), AudioInputTask, ENamedThreads::GameThread);

	FGraphEventRef SttApiEvent = FGraphEvent::CreateGraphEvent();
	FGraphEventRef SttTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[FlowContext, SttApiEvent]()
		{
			SttApiTaskFunction(FlowContext->AudioInput_Result.InputAudioBuffer, SttApiEvent, FlowContext->SttApi_Result);
		}, TStatId(), AudioInputTask, ENamedThreads::AnyThread);


	FGraphEventRef LlmApiEvent = FGraphEvent::CreateGraphEvent();
	FGraphEventRef LlmTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[FlowContext, LlmApiEvent]()
		{
			LlmApiTaskFunction(FlowContext->SttApi_Result.SttMessage, LlmApiEvent, FlowContext->LlmApi_Result);
		}, TStatId(), SttApiEvent, ENamedThreads::AnyThread);


	FGraphEventRef TtsApiEvent = FGraphEvent::CreateGraphEvent();
	FGraphEventRef TtsTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[FlowContext, TtsApiEvent]()
		{
			TtsApiTaskFunction(FlowContext->LlmApi_Result.LlmMessage, TtsApiEvent, FlowContext->TtsApi_Result);
		}, TStatId(), LlmApiEvent, ENamedThreads::AnyThread);


	FGraphEventRef GameTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[FlowContext, World]()
		{
			GameTaskFunction(FlowContext->TtsApi_Result.TtsAudioBuffer, World);
		}, TStatId(), TtsApiEvent, ENamedThreads::GameThread);
}


void ConvAITaskGraphExamples::AudioInputTaskFunction(AudioInputTask_Result& OutResult)
{
	FConvAIModule::LogWithThreadInfo(TEXT("[AudioInputTask] Task started."));
	FString RelativePath = FPaths::ProjectDir() / TEXT("Source/Python/output/audio.wav");
	if (!FConvAIModule::LoadWavFileToBuffer(RelativePath, OutResult.InputAudioBuffer))
	{
		FString LogMsg = FString::Printf(TEXT("[AudioInputTask] Load wave file failed.  Path : %s"), *RelativePath);
		FConvAIModule::LogErrorWithThreadInfo(LogMsg);
	}
}


void ConvAITaskGraphExamples::SttApiTaskFunction(
	const TArray<uint8>& InAudioBuffer, FGraphEventRef FinishEvent, SttApiTask_Result& OutResult)
{
	FConvAIModule::LogWithThreadInfo(TEXT("[SttTask] Task started."));
	TSharedRef<IHttpRequest> Request = FWhisperUtils::CreateRequest(InAudioBuffer);

	Request->OnProcessRequestComplete().BindLambda(
		[FinishEvent, &OutResult](FHttpRequestPtr InRequest, FHttpResponsePtr InResponse, bool bWasSuccessful)
		{
			if (bWasSuccessful && InResponse.IsValid())
			{
				FWhisperResponse OutResponse;
				if (FWhisperUtils::ParseResponse(InResponse, OutResponse))
				{
					OutResult.SttMessage = OutResponse.text;
					FString LogMsg = FString::Printf(TEXT("[SttTask] Whisper response : %s"), *OutResult.SttMessage);
					FConvAIModule::LogWithThreadInfo(LogMsg);
				}
			}

			FinishEvent->DispatchSubsequents();
		});

	// Execute request
	Request->ProcessRequest();
}


void ConvAITaskGraphExamples::LlmApiTaskFunction(
	const FString& InMessage, FGraphEventRef FinishEvent, LlmApiTask_Result& OutResult)
{
	FConvAIModule::LogWithThreadInfo(TEXT("[LlmTask] Task started."));

	FString APIKey = FChatGPTUtils::GetAPIKey();
	TSharedRef<IHttpRequest> Request = FChatGPTUtils::CreateRequest(APIKey, InMessage);

	// Set response callback
	Request->OnProcessRequestComplete().BindLambda(
		[FinishEvent, &OutResult](FHttpRequestPtr InRequest, FHttpResponsePtr InResponse, bool bWasSuccessful)
		{
			if (bWasSuccessful && InResponse.IsValid())
			{
				bool bSuccess = false;
				FChatGPTResponse ChatGPTResponse;
				if (FChatGPTUtils::ParseResponse(InResponse, ChatGPTResponse))
				{
					if (ChatGPTResponse.choices.Num() > 0)
					{
						bSuccess = true;
						OutResult.LlmMessage = ChatGPTResponse.choices[0].message.content;

						FString LogMsg = FString::Printf(TEXT("[LlmTask] Chat gpt response parsed.  %s"), *OutResult.LlmMessage);
						FConvAIModule::LogWithThreadInfo(LogMsg);
					}
				}

				if (!bSuccess)
				{
					OutResult.LlmMessage = TEXT("This is a dummy message shown due to a ChatGPT response failure.");
					FConvAIModule::LogErrorWithThreadInfo(TEXT("[LlmTask] Chat gpt request failed."));
				}
			}

			FinishEvent->DispatchSubsequents();
		});

	// Execute request
	Request->ProcessRequest();
}


void ConvAITaskGraphExamples::TtsApiTaskFunction(
	const FString& InMessage, FGraphEventRef FinishEvent, TtsApiTask_Result& OutResult)
{
	FConvAIModule::LogWithThreadInfo(TEXT("[TtsTask] Task started."));
	FTTSRequestBody Body;
	Body.Data = InMessage;
	TSharedRef<IHttpRequest> Request = FTtsUtils::CreateRequest(Body);

	// Set response callback
	Request->OnProcessRequestComplete().BindLambda(
		[FinishEvent, &OutResult](FHttpRequestPtr InRequest, FHttpResponsePtr InResponse, bool bWasSuccessful)
		{
			if (bWasSuccessful && InResponse.IsValid())
			{
				const TArray<uint8>& TtsAudioBuffer = InResponse->GetContent();
				OutResult.TtsAudioBuffer = TtsAudioBuffer;
				FConvAIModule::LogWithThreadInfo(TEXT("[TtsTask] Tts request done."));
			}

			FinishEvent->DispatchSubsequents();
		});

	// Execute request
	Request->ProcessRequest();
}

void ConvAITaskGraphExamples::GameTaskFunction(const TArray<uint8>& InAudioBuffer, const UWorld* InWorld)
{
	FConvAIModule::LogWithThreadInfo(TEXT("[GameTaskFunction] Task started."));
	if (!IsValid(InWorld))
	{
		FConvAIModule::LogErrorWithThreadInfo(TEXT("[GameTaskFunction] World is not valid."));
		return;
	}

	USoundWaveProcedural* SoundWav = FConvAIModule::CreateSoundWaveFromWav(InAudioBuffer);

	if (IsValid(SoundWav))
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[GameTaskFunction] Play sound."));
		UGameplayStatics::PlaySound2D(InWorld, SoundWav);
	}

}