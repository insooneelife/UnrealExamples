// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WhisperUtils.h"
#include "LLMAPITask.h"
#include "ConvAIModule.h"
#include "Engine/World.h"
#include "Sound/SoundWaveProcedural.h"

class UWorld;
class USoundWaveProcedural;
struct SttApiTask_Result;
struct LlmApiTask_Result;
struct TtsApiTask_Result;


DECLARE_DELEGATE_OneParam(FPostSttApiDelegate, const SttApiTask_Result& /*OutResult*/ );
DECLARE_DELEGATE_OneParam(FPostLlmApiDelegate, const LlmApiTask_Result& /*OutResult*/);
DECLARE_DELEGATE_OneParam(FPostTtsApiDelegate, const TtsApiTask_Result& /*OutResult*/);


struct AudioInputTask_Result
{
	void Reset() { bSuccess  = false; AudioBuffer.Reset(); }
public:
	bool bSuccess;
	TArray<uint8> AudioBuffer;
};

struct SttApiTask_Result
{
	void Reset() { bSuccess = false; Message.Reset(); }
public:
	bool bSuccess;
	FString Message;
};

struct SttDeviceTask_Result
{
	void Reset() { bSuccess = false; Message.Reset(); }
public:
	bool bSuccess;
	FString Message;
};

struct LlmApiTask_Result
{
	void Reset() { bSuccess = false; Message.Reset(); }
public:
	bool bSuccess;
	FString Message;
};

struct LlmDeviceTask_Result
{
	void Reset() { bSuccess = false; Message.Reset(); }
public:
	bool bSuccess;
	FString Message;
};

struct TtsApiTask_Result
{
	void Reset() { bSuccess = false; AudioBuffer.Reset(); }
public:
	bool bSuccess;
	TArray<uint8> AudioBuffer;
};

struct TtsDeviceTask_Result
{
	void Reset() { bSuccess = false; AudioBuffer.Reset(); }
public:
	bool bSuccess;
	TArray<uint8> AudioBuffer;
};

class FGlobalContext : public TSharedFromThis<FGlobalContext, ESPMode::ThreadSafe>
{
public:
	TArray<FString> StackLlmMessages;
	FString LlmMessage;
};

class FApiFlowContext : public TSharedFromThis<FApiFlowContext, ESPMode::ThreadSafe>
{
public:
	void Reset()
	{
		Audio_Result.Reset();
		Stt_Result.Reset();
		Llm_Result.Reset();
		Tts_Result.Reset();
	}

public:

	void OnBegin(TSharedPtr<FGlobalContext> GlobalContext)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FApiFlowContext::OnBegin]"));
	}

	void OnEnd(TSharedPtr<FGlobalContext> GlobalContext)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FApiFlowContext::OnEnd]"));
	}

	void OnPreCollectAudioInput()
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FApiFlowContext::OnPreCollectAudioInput]"));
	}

	void OnPostCollectAudioInput(const AudioInputTask_Result& InResult)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FApiFlowContext::OnPostCollectAudioInput]"));
	}
	
	void OnPreSttApi(const TArray<uint8>& InAudioBuffer)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FApiFlowContext::OnPreSttApi]"));
	}

	void OnPostSttApi(const SttApiTask_Result& InResult)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FApiFlowContext::OnPostSttApi]"));

		if(InResult.bSuccess)
		{
			FString LogMsg = FString::Printf(TEXT("[FApiFlowContext::OnPostSttApi] Stt Response : %s"), *InResult.Message);
			FConvAIModule::LogWithThreadInfo(LogMsg);
		}
	}

	void OnPreLlmApi(const FString& InPrompt)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FApiFlowContext::OnPreLlmApi]"));
	}

	void OnPostLlmApi(const LlmApiTask_Result& InResult)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FApiFlowContext::OnPostLlmApi]"));

		if(InResult.bSuccess)
		{
			FString LogMsg = FString::Printf(TEXT("[FApiFlowContext::OnPostLlmApi] Llm Response. : %s"), *InResult.Message);
			FConvAIModule::LogWithThreadInfo(LogMsg);
		}
	}

	void OnPreTtsApi(const FString& InMessage)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FApiFlowContext::OnPreTtsApi]"));
	}

	void OnPostTtsApi(const TtsApiTask_Result& InResult)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FApiFlowContext::OnPostTtsApi]"));

		if(InResult.bSuccess)
		{
			FString LogMsg = FString::Printf(TEXT("[FApiFlowContext::OnPostTtsApi] Tts Response : %d"), InResult.AudioBuffer.Num());
			FConvAIModule::LogWithThreadInfo(LogMsg);
		}
	}

	// needs to check if world is valid
	void OnPrePlayGameSound(const TArray<uint8>& InAudioBuffer, const UWorld* const InWorld, TSharedPtr<FGlobalContext> GlobalContext)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FApiFlowContext::OnPrePlayGameSound]"));
	}

	void OnPostPlayGameSound(
		const TArray<uint8>& InAudioBuffer, 
		const UWorld* const InWorld, 
		const USoundWaveProcedural* const InSoundWave, 
		TSharedPtr<FGlobalContext> GlobalContext)
	{
		if (!IsValid(InWorld))
		{
			return;
		}

		FConvAIModule::LogWithThreadInfo(TEXT("[FApiFlowContext::OnPostPlayGameSound]"));
		if (IsValid(InSoundWave))
		{
			FConvAIModule::LogWithThreadInfo(TEXT("[FApiFlowContext::OnPostPlayGameSound] Sound Wave Successfully Created."));
		}
	}


public:
	AudioInputTask_Result Audio_Result;
	SttApiTask_Result Stt_Result;
	LlmApiTask_Result Llm_Result;
	TtsApiTask_Result Tts_Result;
};


class FDeviceFlowContext : public TSharedFromThis<FDeviceFlowContext, ESPMode::ThreadSafe>
{
public:
	void Reset()
	{
		Audio_Result.Reset();
		Stt_Result.Reset();
		Llm_Result.Reset();
	}

	void OnBegin(TSharedPtr<FGlobalContext> GlobalContext)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FDeviceFlowContext::OnBegin]"));
	}

	void OnEnd(TSharedPtr<FGlobalContext> GlobalContext)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FDeviceFlowContext::OnEnd]"));
	}

	void OnPreCollectAudioInput()
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FApiFlowContext::OnPreCollectAudioInput]"));
	}

	void OnPostCollectAudioInput(const AudioInputTask_Result& InResult)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FApiFlowContext::OnPostCollectAudioInput]"));
	}


	void OnPreSttDevice(const TArray<uint8>& InAudioBuffer)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FDeviceFlowContext::OnPreSttDevice]"));
	}

	void OnPostSttDevice(const SttDeviceTask_Result& InResult)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FDeviceFlowContext::OnPostSttDevice]"));
	}

	void OnPreLlmDevice(const FString& InPrompt)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FDeviceFlowContext::OnPreLlmDevice]"));
	}

	void OnPostLlmDevice(const LlmDeviceTask_Result& InResult)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FDeviceFlowContext::OnPostLlmDevice]"));
	}

	void OnPreUpdateGameByLlm(const FString& InMessage, const UWorld* const InWorld, TSharedPtr<FGlobalContext> GlobalContext)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FDeviceFlowContext::OnPreUpdateGameByLlm]"));
	}

	void OnPostUpdateGameByLlm(const LlmDeviceTask_Result& InResult, const UWorld* const InWorld, TSharedPtr<FGlobalContext> GlobalContext)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FDeviceFlowContext::OnPostUpdateGameByLlm]"));
	}

public:
	AudioInputTask_Result Audio_Result;
	SttDeviceTask_Result Stt_Result;
	LlmDeviceTask_Result Llm_Result;
};

class FDevicePlaySoundFlowContext : public TSharedFromThis<FDevicePlaySoundFlowContext, ESPMode::ThreadSafe>
{
public:
	void Reset()
	{
		LlmMessage.Reset();
		Tts_Result.Reset();
	}

	void OnBegin(TSharedPtr<FGlobalContext> GlobalContext)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FDevicePlaySoundFlowContext::OnBegin]"));
	}

	void OnEnd(TSharedPtr<FGlobalContext> GlobalContext)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FDevicePlaySoundFlowContext::OnEnd]"));
	}

	void OnPreTtsDevice(const FString& InMessage)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FDevicePlaySoundFlowContext::OnPreTtsDevice]"));
	}

	void OnPostTtsDevice(const TtsDeviceTask_Result& InResult)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FDevicePlaySoundFlowContext::OnPostTtsDevice]"));

		if (InResult.bSuccess)
		{
			FString LogMsg = FString::Printf(TEXT("[FDevicePlaySoundFlowContext::OnPostTtsDevice] Tts Response : %d"), InResult.AudioBuffer.Num());
			FConvAIModule::LogWithThreadInfo(LogMsg);
		}
	}

	void OnPrePlayGameSound(const TArray<uint8>& InAudioBuffer, const UWorld* const InWorld, TSharedPtr<FGlobalContext> GlobalContext)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FDevicePlaySoundFlowContext::OnPrePlayGameSound]"));
	}

	void OnPostPlayGameSound(
		const TArray<uint8>& InAudioBuffer,
		const UWorld* const InWorld,
		const USoundWaveProcedural* const InSoundWave,
		TSharedPtr<FGlobalContext> GlobalContext)
	{
		if (!IsValid(InWorld))
		{
			return;
		}

		FConvAIModule::LogWithThreadInfo(TEXT("[FDevicePlaySoundFlowContext::OnPostPlayGameSound]"));
		if (IsValid(InSoundWave))
		{
			FConvAIModule::LogWithThreadInfo(TEXT("[FDevicePlaySoundFlowContext::OnPostPlayGameSound] Sound Wave Successfully Created."));
		}
	}
public:
	FString LlmMessage;
	TtsDeviceTask_Result Tts_Result;
};

class ConvAITaskGraphExamples
{
public:

	static void AllExamples(UWorld* const World);
	static void TriggerApiFlowTaskGraph(TSharedPtr<FGlobalContext> GlobalContext, UWorld* const World);
	static void TriggerDeviceProducerFlowTaskGraph(TSharedPtr<FGlobalContext> GlobalContext, UWorld* const World);
	static void TriggerDeviceConsumerFlowTaskGraph(TSharedPtr<FGlobalContext> GlobalContext, UWorld* const World);

	static void CollectAudioInput(AudioInputTask_Result& OutResult);
	static void RequestSttApi(const TArray<uint8>& InAudioBuffer, FGraphEventRef FinishEvent, FPostSttApiDelegate EndDelegate, SttApiTask_Result& OutResult);
	static void RequestLlmApi(const FString& InMessage, FGraphEventRef FinishEvent, FPostLlmApiDelegate EndDelegate, LlmApiTask_Result& OutResult);
	static void RequestTtsApi(const FString& InMessage, FGraphEventRef FinishEvent, FPostTtsApiDelegate EndDelegate, TtsApiTask_Result& OutResult);

	static void ProcessSttDevice(const TArray<uint8>& InAudioBuffer, SttDeviceTask_Result& OutResult);
	static void ProcessLlmDevice(const FString& InMessage, LlmDeviceTask_Result& OutResult);
	static void ProcessTtsDevice(const FString& InMessage, TtsDeviceTask_Result& OutResult);

	static USoundWaveProcedural* PlayGameSound(const TArray<uint8>& InAudioBuffer, const UWorld* const InWorld, TSharedPtr<FGlobalContext> GlobalContext);
	static void UpdateGameByLlm(const FString& InLlmMessage, const UWorld* const InWorld, TSharedPtr<FGlobalContext> GlobalContext);
};
