// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WhisperUtils.h"
#include "LLMAPITask.h"

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

	void OnBegin()
	{
		UE_LOG(LogTemp, Error, TEXT("[FApiFlowContext] OnBegin ########"));
	}

	void OnEnd()
	{
		UE_LOG(LogTemp, Error, TEXT("[FApiFlowContext] OnEnd ########"));
	}

	void OnPreCollectAudioInput()
	{
		UE_LOG(LogTemp, Error, TEXT("[FApiFlowContext] OnPreCollectAudioInput ########"));
	}

	void OnPostCollectAudioInput(const AudioInputTask_Result& InResult)
	{
		UE_LOG(LogTemp, Error, TEXT("[FApiFlowContext] OnPostCollectAudioInput ########"));
	}
	
	void OnPreSttApi(const TArray<uint8>& InAudioBuffer)
	{
		UE_LOG(LogTemp, Error, TEXT("[FApiFlowContext] OnPreSttApi ########"));
	}

	void OnPostSttApi(const SttApiTask_Result& InResult)
	{
		UE_LOG(LogTemp, Error, TEXT("[FApiFlowContext] OnPostSttApi ########"));
	}

	void OnPreLlmApi(const FString& InPrompt)
	{
		UE_LOG(LogTemp, Error, TEXT("[FApiFlowContext] OnPreLlmApi ########"));
	}

	void OnPostLlmApi(const LlmApiTask_Result& InResult)
	{
		UE_LOG(LogTemp, Error, TEXT("[FApiFlowContext] OnPostLlmApi ########"));
	}

	void OnPreTtsApi(const FString& InMessage)
	{
		UE_LOG(LogTemp, Error, TEXT("[FApiFlowContext] OnPreTtsApi ########"));
	}

	void OnPostTtsApi(const TtsApiTask_Result& InResult)
	{
		UE_LOG(LogTemp, Error, TEXT("[FApiFlowContext] OnPostTtsApi ########"));
	}

	// needs to check if world is valid
	void OnPrePlayGameSound(const TArray<uint8>& InAudioBuffer, const UWorld* const InWorld, TSharedPtr<FGlobalContext> GlobalContext)
	{
		UE_LOG(LogTemp, Error, TEXT("[FApiFlowContext] OnPrePlayGameSound ########"));
	}

	void OnPostPlayGameSound(
		const TArray<uint8>& InAudioBuffer, 
		const UWorld* const InWorld, 
		const USoundWaveProcedural* const InSoundWave, 
		TSharedPtr<FGlobalContext> GlobalContext)
	{
		UE_LOG(LogTemp, Error, TEXT("[FApiFlowContext] OnPostPlayGameSound ########"));
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

	void OnBegin()
	{
		UE_LOG(LogTemp, Error, TEXT("[FDeviceFlowContext] OnBegin ########"));
	}

	void OnEnd()
	{
		UE_LOG(LogTemp, Error, TEXT("[FDeviceFlowContext] OnEnd ########"));
	}

	void OnPreCollectAudioInput()
	{
		UE_LOG(LogTemp, Error, TEXT("[FDeviceFlowContext] OnPreCollectAudioInput ########"));
	}

	void OnPostCollectAudioInput(const AudioInputTask_Result& InResult)
	{
		UE_LOG(LogTemp, Error, TEXT("[FDeviceFlowContext] OnPostCollectAudioInput ########"));
	}


	void OnPreSttDevice(const TArray<uint8>& InAudioBuffer)
	{
		UE_LOG(LogTemp, Error, TEXT("[FDeviceFlowContext] OnPreSttDevice ########"));
	}

	void OnPostSttDevice(const SttDeviceTask_Result& InResult)
	{
		UE_LOG(LogTemp, Error, TEXT("[FDeviceFlowContext] OnPostSttDevice ########"));
	}

	void OnPreLlmDevice(const FString& InPrompt)
	{
		UE_LOG(LogTemp, Error, TEXT("[FDeviceFlowContext] OnPreLlmDevice ########"));
	}

	void OnPostLlmDevice(const LlmDeviceTask_Result& InResult)
	{
		UE_LOG(LogTemp, Error, TEXT("[FDeviceFlowContext] OnPostLlmDevice ########"));
	}

	void OnPreUpdateGameByLlm(const FString& InMessage, const UWorld* const InWorld, TSharedPtr<FGlobalContext> GlobalContext)
	{
		UE_LOG(LogTemp, Error, TEXT("[FDeviceFlowContext] OnPreUpdateGameByLlm ########"));
	}

	void OnPostUpdateGameByLlm(const LlmDeviceTask_Result& InResult, const UWorld* const InWorld, TSharedPtr<FGlobalContext> GlobalContext)
	{
		UE_LOG(LogTemp, Error, TEXT("[FDeviceFlowContext] OnPostUpdateGameByLlm ########"));
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

	void OnBegin()
	{
		UE_LOG(LogTemp, Error, TEXT("[FDevicePlaySoundFlowContext] OnBegin ########"));
	}

	void OnEnd()
	{
		UE_LOG(LogTemp, Error, TEXT("[FDevicePlaySoundFlowContext] OnEnd ########"));
	}

	void OnPreTtsDevice(const FString& InMessage)
	{
		UE_LOG(LogTemp, Error, TEXT("[FDevicePlaySoundFlowContext] OnPreTtsDevice ########"));
	}

	void OnPostTtsDevice(const TtsDeviceTask_Result& InResult)
	{
		UE_LOG(LogTemp, Error, TEXT("[FDevicePlaySoundFlowContext] OnPostTtsDevice ########"));
	}

	void OnPrePlayGameSound(const TArray<uint8>& InAudioBuffer, const UWorld* const InWorld, TSharedPtr<FGlobalContext> GlobalContext)
	{
		UE_LOG(LogTemp, Error, TEXT("[FDevicePlaySoundFlowContext] OnPrePlayGameSound ########"));
	}

	void OnPostPlayGameSound(
		const TArray<uint8>& InAudioBuffer,
		const UWorld* const InWorld,
		const USoundWaveProcedural* const InSoundWave,
		TSharedPtr<FGlobalContext> GlobalContext)
	{
		UE_LOG(LogTemp, Error, TEXT("[FDevicePlaySoundFlowContext] OnPostPlayGameSound ########"));
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
