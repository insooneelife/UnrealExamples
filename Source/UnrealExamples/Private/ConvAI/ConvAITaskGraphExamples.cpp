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
	TSharedPtr<FGlobalContext> GlobalContext = MakeShared<FGlobalContext>();

	TriggerApiFlowTaskGraph(GlobalContext, World);
	TriggerDeviceProducerFlowTaskGraph(GlobalContext, World);
}


void ConvAITaskGraphExamples::TriggerApiFlowTaskGraph(TSharedPtr<FGlobalContext> GlobalContext, UWorld* World)
{
	if (!IsValid(World))
	{
		FConvAIModule::LogErrorWithThreadInfo(TEXT("[TriggerApiFlowTaskGraph] World is not valid."));
		return;
	}

	TSharedPtr<FApiFlowContext> Context = MakeShared<FApiFlowContext>();
	Context->Reset();

	FGraphEventRef AudioInputTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[Context]()
		{
			AudioInputTaskFunction(Context->Audio_Result);
		}, TStatId(), nullptr, ENamedThreads::AnyThread);

	FGraphEventRef InputAudioGameTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[GlobalContext, Context, World]()
		{
			if(Context->Audio_Result.bSuccess)
			{
				PlayAudioGameTaskFunction(GlobalContext, Context->Audio_Result.AudioBuffer, World);
			}
		}, TStatId(), AudioInputTask, ENamedThreads::GameThread);

	FGraphEventRef SttApiEvent = FGraphEvent::CreateGraphEvent();
	FGraphEventRef SttTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[Context, SttApiEvent]()
		{
			if (Context->Audio_Result.bSuccess)
			{
				SttApiTaskFunction(Context->Audio_Result.AudioBuffer, SttApiEvent, Context->Stt_Result);
			}
			else
			{
				SttApiEvent->DispatchSubsequents();
			}
		}, TStatId(), AudioInputTask, ENamedThreads::AnyThread);


	FGraphEventRef LlmApiEvent = FGraphEvent::CreateGraphEvent();
	FGraphEventRef LlmTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[Context, LlmApiEvent]()
		{
			if (Context->Stt_Result.bSuccess)
			{
				LlmApiTaskFunction(Context->Stt_Result.Message, LlmApiEvent, Context->Llm_Result);
			}
			else
			{
				LlmApiEvent->DispatchSubsequents();
			}

		}, TStatId(), SttApiEvent, ENamedThreads::AnyThread);


	FGraphEventRef TtsApiEvent = FGraphEvent::CreateGraphEvent();
	FGraphEventRef TtsTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[Context, TtsApiEvent]()
		{
			if (Context->Llm_Result.bSuccess)
			{
				TtsApiTaskFunction(Context->Llm_Result.Message, TtsApiEvent, Context->Tts_Result);
			}
			else
			{
				TtsApiEvent->DispatchSubsequents();
			}
		}, TStatId(), LlmApiEvent, ENamedThreads::AnyThread);


	FGraphEventRef GameTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[GlobalContext, Context, World]()
		{
			if (Context->Tts_Result.bSuccess)
			{
				PlayAudioGameTaskFunction(GlobalContext, Context->Tts_Result.AudioBuffer, World);
			}

			// trigger this cycle again.
			TriggerApiFlowTaskGraph(GlobalContext, World);
		}, TStatId(), TtsApiEvent, ENamedThreads::GameThread);
}

void ConvAITaskGraphExamples::TriggerDeviceProducerFlowTaskGraph(
	TSharedPtr<FGlobalContext> GlobalContext, UWorld* World)
{
	if (!IsValid(World))
	{
		FConvAIModule::LogErrorWithThreadInfo(TEXT("[TriggerDeviceProducerFlowTaskGraph] World is not valid."));
		return;
	}

	TSharedPtr<FDeviceFlowContext> Context = MakeShared<FDeviceFlowContext>();
	Context->Reset();

	FGraphEventRef AudioInputTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[Context]()
		{
			AudioInputTaskFunction(Context->Audio_Result);
		}, TStatId(), nullptr, ENamedThreads::AnyThread);

	FGraphEventRef SttTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[Context]()
		{
			if (Context->Audio_Result.bSuccess)
			{
				SttDeviceTaskFunction(Context->Audio_Result.AudioBuffer, Context->Stt_Result);
			}
		}, TStatId(), AudioInputTask, ENamedThreads::AnyThread);

	FGraphEventRef LlmTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[Context]()
		{
			if (Context->Stt_Result.bSuccess)
			{
				LlmDeviceTaskFunction(Context->Stt_Result.Message, Context->Llm_Result);
			}

		}, TStatId(), SttTask, ENamedThreads::AnyThread);

	FGraphEventRef GameTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[GlobalContext, Context, World]()
		{
			if (Context->Tts_Result.bSuccess)
			{
				UpdateLlmOutputGameTaskFunction(GlobalContext, Context->Llm_Result.Message, World);
				TriggerDeviceConsumerFlowTaskGraph(GlobalContext, World);
			}
			
			// trigger this cycle again.
			TriggerDeviceProducerFlowTaskGraph(GlobalContext, World);
		}, TStatId(), LlmTask, ENamedThreads::GameThread);


}

void ConvAITaskGraphExamples::TriggerDeviceConsumerFlowTaskGraph(TSharedPtr<FGlobalContext> GlobalContext, UWorld* World)
{	
	if (!IsValid(World))
	{
		FConvAIModule::LogErrorWithThreadInfo(TEXT("[TriggerDeviceConsumerFlowTaskGraph] World is not valid."));
		return;
	}

	TSharedPtr<FDeviceFlowContext> Context = MakeShared<FDeviceFlowContext>();
	FGraphEventRef TtsTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[Context]()
		{
			if (Context->Llm_Result.bSuccess)
			{
				TtsDeviceTaskFunction(Context->Llm_Result.Message, Context->Tts_Result);
			}
		}, TStatId(), nullptr, ENamedThreads::AnyThread);


	FGraphEventRef GameTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[GlobalContext, Context, World]()
		{
			if (Context->Tts_Result.bSuccess)
			{
				PlayAudioGameTaskFunction(GlobalContext, Context->Tts_Result.AudioBuffer, World);
			}
		}, TStatId(), TtsTask, ENamedThreads::GameThread);	
}

void ConvAITaskGraphExamples::AudioInputTaskFunction(AudioInputTask_Result& OutResult)
{
	FConvAIModule::LogWithThreadInfo(TEXT("[AudioInputTask] Task started."));
	FString RelativePath = FPaths::ProjectDir() / TEXT("Source/Python/output/audio.wav");

	if (FConvAIModule::LoadWavFileToBuffer(RelativePath, OutResult.AudioBuffer))
	{
		OutResult.bSuccess = true;
	}
	else
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
					OutResult.bSuccess = true;
					OutResult.Message = OutResponse.text;
					FString LogMsg = FString::Printf(TEXT("[SttTask] Whisper response : %s"), *OutResult.Message);
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
				FChatGPTResponse ChatGPTResponse;
				if (FChatGPTUtils::ParseResponse(InResponse, ChatGPTResponse))
				{
					if (ChatGPTResponse.choices.Num() > 0)
					{
						OutResult.bSuccess = true;
						OutResult.Message = ChatGPTResponse.choices[0].message.content;

						FString LogMsg = FString::Printf(TEXT("[LlmTask] Chat gpt response parsed.  %s"), *OutResult.Message);
						FConvAIModule::LogWithThreadInfo(LogMsg);
					}
				}

				if (!OutResult.bSuccess)
				{
					OutResult.Message = TEXT("This is a dummy message shown due to a ChatGPT response failure.");
					FConvAIModule::LogErrorWithThreadInfo(TEXT("[LlmTask] Chat gpt request failed."));

					// set this true for just example.
					OutResult.bSuccess = true;
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
				OutResult.bSuccess = true;
				const TArray<uint8>& TtsAudioBuffer = InResponse->GetContent();
				OutResult.AudioBuffer = TtsAudioBuffer;
				FConvAIModule::LogWithThreadInfo(TEXT("[TtsTask] Tts request done."));
			}

			FinishEvent->DispatchSubsequents();
		});

	// Execute request
	Request->ProcessRequest();
}


void ConvAITaskGraphExamples::SttDeviceTaskFunction(const TArray<uint8>& InAudioBuffer, SttDeviceTask_Result& OutResult)
{
	FConvAIModule::LogWithThreadInfo(TEXT("[SttDeviceTask] Task started."));

	// implement GlobalSttDevice->Inference()
	FPlatformProcess::Sleep(1);

	// any message
	OutResult.bSuccess = true;
	OutResult.Message = TEXT("some stt message created by device with stt model.");
}

void ConvAITaskGraphExamples::LlmDeviceTaskFunction(const FString& InMessage, LlmDeviceTask_Result& OutResult)
{
	FConvAIModule::LogWithThreadInfo(TEXT("[LlmDeviceTask] Task started."));

	// implement GlobalLlmDevice->Inference()
	FPlatformProcess::Sleep(1);

	// any message
	OutResult.bSuccess = true;
	OutResult.Message = TEXT("some llm message created by device with stt model.");
}

void ConvAITaskGraphExamples::TtsDeviceTaskFunction(const FString& InMessage, TtsDeviceTask_Result& OutResult)
{
	FConvAIModule::LogWithThreadInfo(TEXT("[TtsDeviceTask] Task started."));

	// implement GlobalTtsDevice->Inference()
	FPlatformProcess::Sleep(1);

	// any audio
	OutResult.bSuccess = true;
	FString RelativePath = FPaths::ProjectDir() / TEXT("Source/Python/output/audio.wav");
	if (FConvAIModule::LoadWavFileToBuffer(RelativePath, OutResult.AudioBuffer)){}
}

void ConvAITaskGraphExamples::PlayAudioGameTaskFunction(
	TSharedPtr<FGlobalContext> GlobalContext, const TArray<uint8>& InAudioBuffer, const UWorld* InWorld)
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


void ConvAITaskGraphExamples::UpdateLlmOutputGameTaskFunction(
	TSharedPtr<FGlobalContext> GlobalContext, const FString& InLlmMessage, const UWorld* InWorld)
{
	GlobalContext->StackLlmMessages.Add(InLlmMessage);

	if(GlobalContext->StackLlmMessages.Num() > 3)
	{
		GlobalContext->StackLlmMessages[1];
		GlobalContext->StackLlmMessages.Empty();
	}
}