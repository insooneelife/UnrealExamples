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
			AudioInputTaskFunction(FlowContext->InputAudioBuffer);
		}, TStatId(), nullptr, ENamedThreads::AnyThread);

	FGraphEventRef InputAudioGameTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[FlowContext, World]()
		{
			GameTaskFunction(FlowContext->InputAudioBuffer, World);
		}, TStatId(), AudioInputTask, ENamedThreads::GameThread);

	FGraphEventRef SttApiEvent = FGraphEvent::CreateGraphEvent();
	FGraphEventRef SttTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[FlowContext, SttApiEvent]()
		{
			SttApiTaskFunction(FlowContext->InputAudioBuffer, SttApiEvent, FlowContext->SttOutputMessage);
		}, TStatId(), AudioInputTask, ENamedThreads::AnyThread);


	FGraphEventRef LlmApiEvent = FGraphEvent::CreateGraphEvent();
	FGraphEventRef LlmTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[FlowContext, LlmApiEvent]()
		{
			LlmApiTaskFunction(FlowContext->SttOutputMessage, LlmApiEvent, FlowContext->LlmMessage);
		}, TStatId(), SttApiEvent, ENamedThreads::AnyThread);


	FGraphEventRef TtsApiEvent = FGraphEvent::CreateGraphEvent();
	FGraphEventRef TtsTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[FlowContext, TtsApiEvent]()
		{
			TtsApiTaskFunction(FlowContext->LlmMessage, TtsApiEvent, FlowContext->TtsAudioBuffer);
		}, TStatId(), LlmApiEvent, ENamedThreads::AnyThread);


	FGraphEventRef GameTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[FlowContext, World]()
		{
			GameTaskFunction(FlowContext->TtsAudioBuffer, World);
		}, TStatId(), TtsApiEvent, ENamedThreads::GameThread);
}





void ConvAITaskGraphExamples::AudioInputTaskFunction(TArray<uint8>& OutAudioBuffer)
{
	FConvAIModule::LogWithThreadInfo(TEXT("[AudioInputTask] Task started."));
	FString RelativePath = FPaths::ProjectDir() / TEXT("Source/Python/output/audio.wav");
	if (!FConvAIModule::LoadWavFileToBuffer(RelativePath, OutAudioBuffer))
	{
		FString LogMsg = FString::Printf(TEXT("[AudioInputTask] Load wave file failed.  Path : %s"), *RelativePath);
		FConvAIModule::LogErrorWithThreadInfo(LogMsg);
	}
}


void ConvAITaskGraphExamples::SttApiTaskFunction(
	const TArray<uint8>& InAudioBuffer, FGraphEventRef FinishEvent, FString& OutSttMessage)
{
	FConvAIModule::LogWithThreadInfo(TEXT("[SttTask] Task started."));
	TSharedRef<IHttpRequest> Request = FWhisperUtils::CreateRequest(InAudioBuffer);

	Request->OnProcessRequestComplete().BindLambda(
		[FinishEvent, &OutSttMessage](FHttpRequestPtr InRequest, FHttpResponsePtr InResponse, bool bWasSuccessful)
		{
			if (bWasSuccessful && InResponse.IsValid())
			{
				FWhisperResponse OutResponse;
				if (FWhisperUtils::ParseResponse(InResponse, OutResponse))
				{
					OutSttMessage = OutResponse.text;
					FString LogMsg = FString::Printf(TEXT("[SttTask] Whisper response : %s"), *OutSttMessage);
					FConvAIModule::LogWithThreadInfo(LogMsg);
				}
			}

			FinishEvent->DispatchSubsequents();
		});

	// Execute request
	Request->ProcessRequest();
}


void ConvAITaskGraphExamples::LlmApiTaskFunction(
	const FString& InMessage, FGraphEventRef FinishEvent, FString& OutLlmMessage)
{
	FConvAIModule::LogWithThreadInfo(TEXT("[LlmTask] Task started."));

	FString APIKey = FChatGPTUtils::GetAPIKey();
	TSharedRef<IHttpRequest> Request = FChatGPTUtils::CreateRequest(APIKey, InMessage);

	// Set response callback
	Request->OnProcessRequestComplete().BindLambda(
		[FinishEvent, &OutLlmMessage](FHttpRequestPtr InRequest, FHttpResponsePtr InResponse, bool bWasSuccessful)
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
						OutLlmMessage = ChatGPTResponse.choices[0].message.content;

						FString LogMsg = FString::Printf(TEXT("[LlmTask] Chat gpt response parsed.  %s"), *OutLlmMessage);
						FConvAIModule::LogWithThreadInfo(LogMsg);
					}
				}

				if (!bSuccess)
				{
					OutLlmMessage = TEXT("This is a dummy message shown due to a ChatGPT response failure.");
					FConvAIModule::LogErrorWithThreadInfo(TEXT("[LlmTask] Chat gpt request failed."));
				}
			}

			FinishEvent->DispatchSubsequents();
		});

	// Execute request
	Request->ProcessRequest();
}

void ConvAITaskGraphExamples::TtsApiTaskFunction(
	const FString& InMessage, FGraphEventRef FinishEvent, TArray<uint8>& OutAudioBuffer)
{
	FConvAIModule::LogWithThreadInfo(TEXT("[TtsTask] Task started."));
	FTTSRequestBody Body;
	Body.Data = InMessage;
	TSharedRef<IHttpRequest> Request = FTtsUtils::CreateRequest(Body);

	// Set response callback
	Request->OnProcessRequestComplete().BindLambda(
		[FinishEvent, &OutAudioBuffer](FHttpRequestPtr InRequest, FHttpResponsePtr InResponse, bool bWasSuccessful)
		{
			if (bWasSuccessful && InResponse.IsValid())
			{
				const TArray<uint8>& TtsAudioBuffer = InResponse->GetContent();
				OutAudioBuffer = TtsAudioBuffer;
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