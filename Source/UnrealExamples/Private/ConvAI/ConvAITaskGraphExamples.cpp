// Fill out your copyright notice in the Description page of Project Settings.


#include "ConvAI/ConvAITaskGraphExamples.h"

#include "ConvAIModule.h"
#include "TtsUtils.h"

#include "Http.h"
#include "Misc/Paths.h"

#include "Async/Async.h"
#include "Kismet/GameplayStatics.h"
#include "Audio.h"

#include "HAL/PlatformProcess.h"
#include "Async/TaskGraphInterfaces.h"


void ConvAITaskGraphExamples::AllExamples(UWorld* const World)
{
	TSharedPtr<FGlobalContext> GlobalContext = MakeShared<FGlobalContext>();

	TriggerApiTaskFlow(GlobalContext, World);
	TriggerDeviceProducerTaskFlow(GlobalContext, World);
}

// TriggerApiFlowTaskGraph
//		|
//		v
// [AudioInputTask]
//		|
//		+--------------------------+
//		|                          |
//		v                          v
// [InputAudioGameTask]		   [SttTask]
//								   |
//								   v
//							   [LlmTask]
//								   |
//								   v
//							   [TtsTask]
//								   |
//								   v
//							   [GameTask]
//								   |
//								   v
//						TriggerApiFlowTaskGraph

void ConvAITaskGraphExamples::TriggerApiTaskFlow(TSharedPtr<FGlobalContext> GlobalContext, UWorld* const World)
{
	if (!IsValid(World))
	{
		FConvAIModule::LogErrorWithThreadInfo(TEXT("[TriggerApiTaskFlow] World is not valid."));
		return;
	}

	TSharedPtr<FApiFlowContext> Context = MakeShared<FApiFlowContext>();
	Context->OnBegin(GlobalContext);

	FGraphEventRef AudioInputTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[Context]()
		{
			Context->OnPreCollectAudioInput();
			CollectAudioInput(Context->Audio_Result);
			Context->OnPostCollectAudioInput(Context->Audio_Result);

		}, TStatId(), nullptr, ENamedThreads::AnyThread);

	FGraphEventRef InputAudioGameTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[GlobalContext, Context, World]()
		{
			if (!Context->Audio_Result.bSuccess) 
			{
				return;
			}

			const TArray<uint8>& AudioBuffer = Context->Audio_Result.AudioBuffer;
			Context->OnPrePlayGameSound(AudioBuffer, World, GlobalContext);
			USoundWaveProcedural* SoundWave = PlayGameSound(AudioBuffer, World, GlobalContext);
			Context->OnPostPlayGameSound(AudioBuffer, World, SoundWave, GlobalContext);

		}, TStatId(), AudioInputTask, ENamedThreads::GameThread);

	FGraphEventRef SttApiEvent = FGraphEvent::CreateGraphEvent();
	FGraphEventRef SttTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[Context, SttApiEvent]()
		{
			if (!Context->Audio_Result.bSuccess)
			{
				SttApiEvent->DispatchSubsequents();
				return;
			}

			Context->OnPreSttApi(Context->Audio_Result.AudioBuffer);
			FPostSttApiDelegate EndDelegate = FPostSttApiDelegate::CreateSP(Context.Get(), &FApiFlowContext::OnPostSttApi);
			RequestSttApi(Context->Audio_Result.AudioBuffer, SttApiEvent, EndDelegate, Context->Stt_Result);

		}, TStatId(), AudioInputTask, ENamedThreads::AnyThread);

	FGraphEventRef LlmApiEvent = FGraphEvent::CreateGraphEvent();
	FGraphEventRef LlmTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[Context, LlmApiEvent]()
		{
			if (!Context->Stt_Result.bSuccess)
			{
				LlmApiEvent->DispatchSubsequents();
				return;
			}
			
			Context->OnPreLlmApi(Context->Stt_Result.Message);
			FPostLlmApiDelegate EndDelegate = FPostLlmApiDelegate::CreateSP(Context.Get(), &FApiFlowContext::OnPostLlmApi);
			RequestLlmApi(Context->Stt_Result.Message, LlmApiEvent, EndDelegate, Context->Llm_Result);

		}, TStatId(), SttApiEvent, ENamedThreads::AnyThread);


	FGraphEventRef TtsApiEvent = FGraphEvent::CreateGraphEvent();
	FGraphEventRef TtsTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[Context, TtsApiEvent]()
		{
			if (!Context->Llm_Result.bSuccess)
			{
				TtsApiEvent->DispatchSubsequents();
				return;
			}

			Context->OnPreTtsApi(Context->Llm_Result.Message);
			FPostTtsApiDelegate EndDelegate = FPostTtsApiDelegate::CreateSP(Context.Get(), &FApiFlowContext::OnPostTtsApi);
			RequestTtsApi(Context->Llm_Result.Message, TtsApiEvent, EndDelegate, Context->Tts_Result);
			
		}, TStatId(), LlmApiEvent, ENamedThreads::AnyThread);


	FGraphEventRef GameTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[GlobalContext, Context, World]()
		{
			if (Context->Tts_Result.bSuccess)
			{
				const TArray<uint8>& AudioBuffer = Context->Tts_Result.AudioBuffer;
				Context->OnPrePlayGameSound(AudioBuffer, World, GlobalContext);
				USoundWaveProcedural* SoundWave = PlayGameSound(AudioBuffer, World, GlobalContext);
				Context->OnPostPlayGameSound(AudioBuffer, World, SoundWave, GlobalContext);
			}

			Context->OnEnd(GlobalContext);

			// trigger this cycle again.
			TriggerApiTaskFlow(GlobalContext, World);
		}, TStatId(), TtsApiEvent, ENamedThreads::GameThread);
}


// TriggerDeviceProducerFlowTaskGraph
//		|
//		v
// [AudioInputTask]
//		|
//		v
// [SttTask]
//		|
//		v
// [LlmTask]
//		|
//		v
// [GameTask]
//		|
//		+-------------------------------------------+
//		|											|
//		v											v
// TriggerDeviceProducerFlowTaskGraph		(if success) TriggerDeviceConsumerFlowTaskGraph

void ConvAITaskGraphExamples::TriggerDeviceProducerTaskFlow(
	TSharedPtr<FGlobalContext> GlobalContext, UWorld* const World)
{
	if (!IsValid(World))
	{
		FConvAIModule::LogErrorWithThreadInfo(TEXT("[TriggerDeviceProducerTaskFlow] World is not valid."));
		return;
	}


	TSharedPtr<FDeviceFlowContext> Context = MakeShared<FDeviceFlowContext>();
	Context->OnBegin(GlobalContext);

	FGraphEventRef AudioInputTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[Context]()
		{
			Context->OnPreCollectAudioInput();
			CollectAudioInput(Context->Audio_Result);
			Context->OnPostCollectAudioInput(Context->Audio_Result);
		}, TStatId(), nullptr, ENamedThreads::AnyThread);

	FGraphEventRef SttTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[Context]()
		{
			if (Context->Audio_Result.bSuccess)
			{
				const TArray<uint8>& AudioBuffer = Context->Audio_Result.AudioBuffer;
				Context->OnPreSttDevice(AudioBuffer);
				ProcessSttDevice(AudioBuffer, Context->Stt_Result);
				Context->OnPostSttDevice(Context->Stt_Result);
			}
		}, TStatId(), AudioInputTask, ENamedThreads::AnyThread);

	FGraphEventRef LlmTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[Context]()
		{
			if (Context->Stt_Result.bSuccess)
			{
				const FString& Message = Context->Stt_Result.Message;
				Context->OnPreLlmDevice(Message);
				ProcessLlmDevice(Message, Context->Llm_Result);
				Context->OnPostLlmDevice(Context->Llm_Result);
			}

		}, TStatId(), SttTask, ENamedThreads::AnyThread);

	FGraphEventRef GameTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[GlobalContext, Context, World]()
		{
			if (Context->Llm_Result.bSuccess)
			{
				const FString& Message = Context->Llm_Result.Message;
				Context->OnPreUpdateGameByLlm(Message, World, GlobalContext);
				UpdateGameByLlm(Message, World, GlobalContext);
				Context->OnPostUpdateGameByLlm(Context->Llm_Result, World, GlobalContext);

				TriggerDeviceConsumerTaskFlow(GlobalContext, World);
			}
			
			Context->OnEnd(GlobalContext);

			// trigger this cycle again.
			TriggerDeviceProducerTaskFlow(GlobalContext, World);
		}, TStatId(), LlmTask, ENamedThreads::GameThread);


}


// TriggerDeviceConsumerFlowTaskGraph
//		|
//		v
// [TtsTask]
//		|
//		v
// [GameTask]
//

void ConvAITaskGraphExamples::TriggerDeviceConsumerTaskFlow(
	TSharedPtr<FGlobalContext> GlobalContext, UWorld* const World)
{	
	if (!IsValid(World))
	{
		FConvAIModule::LogErrorWithThreadInfo(TEXT("[TriggerDeviceConsumerTaskFlow] World is not valid."));
		return;
	}

	TSharedPtr<FDevicePlaySoundFlowContext> Context = MakeShared<FDevicePlaySoundFlowContext>();
	Context->LlmMessage = GlobalContext->LlmMessage;
	Context->OnBegin(GlobalContext);

	FGraphEventRef TtsTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[Context]()
		{			
			const FString& Message = Context->LlmMessage;
			Context->OnPreTtsDevice(Message);
			ProcessTtsDevice(Message, Context->Tts_Result);
			Context->OnPostTtsDevice(Context->Tts_Result);
			
		}, TStatId(), nullptr, ENamedThreads::AnyThread);


	FGraphEventRef GameTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[GlobalContext, Context, World]()
		{
			if (Context->Tts_Result.bSuccess)
			{
				const TArray<uint8>& AudioBuffer = Context->Tts_Result.AudioBuffer;
				Context->OnPrePlayGameSound(AudioBuffer, World, GlobalContext);
				USoundWaveProcedural* SoundWave = PlayGameSound(AudioBuffer, World, GlobalContext);
				Context->OnPostPlayGameSound(AudioBuffer, World, SoundWave, GlobalContext);
			}

			Context->OnEnd(GlobalContext);
		}, TStatId(), TtsTask, ENamedThreads::GameThread);	
}

void ConvAITaskGraphExamples::CollectAudioInput(AudioInputTask_Result& OutResult)
{
	FString RelativePath = FPaths::ProjectDir() / TEXT("Source/Python/output/audio.wav");
	if (FConvAIModule::LoadWavFileToBuffer(RelativePath, OutResult.AudioBuffer))
	{
		OutResult.bSuccess = true;
	}
	else
	{
		FString LogMsg = FString::Printf(TEXT("[CollectAudioInput] Load wave file failed.  Path : %s"), *RelativePath);
		FConvAIModule::LogErrorWithThreadInfo(LogMsg);
	}
}


void ConvAITaskGraphExamples::RequestSttApi(
	const TArray<uint8>& InAudioBuffer, 
	FGraphEventRef FinishEvent, 
	FPostSttApiDelegate EndDelegate,
	SttApiTask_Result& OutResult)
{
	TSharedRef<IHttpRequest> Request = FWhisperUtils::CreateRequest(InAudioBuffer);

	Request->OnProcessRequestComplete().BindLambda(
		[FinishEvent, EndDelegate, &OutResult](FHttpRequestPtr InRequest, FHttpResponsePtr InResponse, bool bWasSuccessful)
		{
			if (bWasSuccessful && InResponse.IsValid())
			{
				FWhisperResponse OutResponse;
				if (FWhisperUtils::ParseResponse(InResponse, OutResponse))
				{
					OutResult.bSuccess = true;
					OutResult.Message = OutResponse.text;
				}
			}

			EndDelegate.ExecuteIfBound(OutResult);
			FinishEvent->DispatchSubsequents();
		});

	// Execute request
	Request->ProcessRequest();
}


void ConvAITaskGraphExamples::RequestLlmApi(
	const FString& InMessage, 
	FGraphEventRef FinishEvent,
	FPostLlmApiDelegate EndDelegate,
	LlmApiTask_Result& OutResult)
{
	FString APIKey = FChatGPTUtils::GetAPIKey();
	TSharedRef<IHttpRequest> Request = FChatGPTUtils::CreateRequest(APIKey, InMessage);

	// Set response callback
	Request->OnProcessRequestComplete().BindLambda(
		[FinishEvent, EndDelegate, &OutResult](FHttpRequestPtr InRequest, FHttpResponsePtr InResponse, bool bWasSuccessful)
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
					}
				}

				if (!OutResult.bSuccess)
				{
					OutResult.Message = TEXT("This is a dummy message shown due to a ChatGPT response failure.");
					FConvAIModule::LogErrorWithThreadInfo(TEXT("[RequestLlmApi] Chat gpt request failed."));

					// set this true for just example.
					OutResult.bSuccess = true;
				}
			}

			EndDelegate.ExecuteIfBound(OutResult);
			FinishEvent->DispatchSubsequents();
		});

	// Execute request
	Request->ProcessRequest();
}


void ConvAITaskGraphExamples::RequestTtsApi(
	const FString& InMessage, 
	FGraphEventRef FinishEvent,
	FPostTtsApiDelegate EndDelegate,
	TtsApiTask_Result& OutResult)
{
	FTTSRequestBody Body;
	Body.Data = InMessage;
	TSharedRef<IHttpRequest> Request = FTtsUtils::CreateRequest(Body);

	// Set response callback
	Request->OnProcessRequestComplete().BindLambda(
		[FinishEvent, EndDelegate, &OutResult](FHttpRequestPtr InRequest, FHttpResponsePtr InResponse, bool bWasSuccessful)
		{
			if (bWasSuccessful && InResponse.IsValid())
			{
				OutResult.bSuccess = true;
				const TArray<uint8>& TtsAudioBuffer = InResponse->GetContent();
				OutResult.AudioBuffer = TtsAudioBuffer;
			}

			EndDelegate.ExecuteIfBound(OutResult);
			FinishEvent->DispatchSubsequents();
		});

	// Execute request
	Request->ProcessRequest();
}


void ConvAITaskGraphExamples::ProcessSttDevice(const TArray<uint8>& InAudioBuffer, SttDeviceTask_Result& OutResult)
{
	// implement GlobalSttDevice->Inference()
	FPlatformProcess::Sleep(1);

	// any message
	OutResult.bSuccess = true;
	OutResult.Message = TEXT("some stt message created by device with stt model.");
}

void ConvAITaskGraphExamples::ProcessLlmDevice(const FString& InMessage, LlmDeviceTask_Result& OutResult)
{
	// implement GlobalLlmDevice->Inference()
	FPlatformProcess::Sleep(1);

	// any message
	OutResult.bSuccess = true;
	OutResult.Message = TEXT("some llm message created by device with stt model.");
}

void ConvAITaskGraphExamples::ProcessTtsDevice(const FString& InMessage, TtsDeviceTask_Result& OutResult)
{
	// implement GlobalTtsDevice->Inference()
	FPlatformProcess::Sleep(1);

	// any audio
	OutResult.bSuccess = true;
	FString RelativePath = FPaths::ProjectDir() / TEXT("Source/Python/output/audio.wav");
	if (FConvAIModule::LoadWavFileToBuffer(RelativePath, OutResult.AudioBuffer)){}
}

USoundWaveProcedural* ConvAITaskGraphExamples::PlayGameSound(
	const TArray<uint8>& InAudioBuffer, const UWorld* const InWorld, TSharedPtr<FGlobalContext> GlobalContext)
{
	if (!IsValid(InWorld))
	{
		FConvAIModule::LogErrorWithThreadInfo(TEXT("[ProcessTtsDevice] World is not valid."));
		return nullptr;
	}

	USoundWaveProcedural* SoundWav = FConvAIModule::CreateSoundWaveFromWav(InAudioBuffer);
	if (IsValid(SoundWav))
	{
		UGameplayStatics::PlaySound2D(InWorld, SoundWav);
		return SoundWav;
	}

	return nullptr;
}


void ConvAITaskGraphExamples::UpdateGameByLlm(
	const FString& InLlmMessage, const UWorld* const InWorld, TSharedPtr<FGlobalContext> GlobalContext)
{
	GlobalContext->StackLlmMessages.Add(InLlmMessage);

	if(GlobalContext->StackLlmMessages.Num() > 3)
	{
		GlobalContext->LlmMessage = GlobalContext->StackLlmMessages[1];
		GlobalContext->StackLlmMessages.Empty();
	}
}