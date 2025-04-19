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

	void OnPreCollectAudioInput()
	{
		UE_LOG(LogTemp, Error, TEXT("OnPreCollectAudioInput ########"));
	}

	void OnPostCollectAudioInput(const AudioInputTask_Result& InResult)
	{
		UE_LOG(LogTemp, Error, TEXT("OnPostCollectAudioInput ########"));
	}
	
	void OnPreSttApi(const TArray<uint8>& InAudioBuffer)
	{
		UE_LOG(LogTemp, Error, TEXT("OnPreSttApi ########"));
	}

	void OnPostSttApi(const SttApiTask_Result& InResult)
	{
		UE_LOG(LogTemp, Error, TEXT("OnPostSttApi ########"));
	}

	void OnPreLlmApi(const FString& InPrompt)
	{
		UE_LOG(LogTemp, Error, TEXT("OnPreLlmApi ########"));
	}

	void OnPostLlmApi(const LlmApiTask_Result& InResult)
	{
		UE_LOG(LogTemp, Error, TEXT("OnPostLlmApi ########"));
	}

	void OnPreTtsApi(const FString& InMessage)
	{
		UE_LOG(LogTemp, Error, TEXT("OnPreTtsApi ########"));
	}

	void OnPostTtsApi(const TtsApiTask_Result& InResult)
	{
		UE_LOG(LogTemp, Error, TEXT("OnPostTtsApi ########"));
	}

	// needs to check if world is valid
	void OnPrePlayGameSound(const TArray<uint8>& InAudioBuffer, const UWorld* const InWorld, TSharedPtr<FGlobalContext> GlobalContext)
	{
		UE_LOG(LogTemp, Error, TEXT("OnPrePlayGameSound ########"));
	}

	void OnPostPlayGameSound(
		const TArray<uint8>& InAudioBuffer, 
		const UWorld* const InWorld, 
		const USoundWaveProcedural* const InSoundWave, 
		TSharedPtr<FGlobalContext> GlobalContext)
	{
		UE_LOG(LogTemp, Error, TEXT("OnPostPlayGameSound ########"));
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
		Tts_Result.Reset();
	}

	void OnPreCollectAudioInput()
	{
		UE_LOG(LogTemp, Error, TEXT("OnPreCollectAudioInput ########"));
	}

	void OnPostCollectAudioInput(const AudioInputTask_Result& InResult)
	{
		UE_LOG(LogTemp, Error, TEXT("OnPostCollectAudioInput ########"));
	}


	void OnPreSttDevice(const TArray<uint8>& InAudioBuffer)
	{
		UE_LOG(LogTemp, Error, TEXT("OnPreSttDevice ########"));
	}

	void OnPostSttDevice(const SttDeviceTask_Result& InResult)
	{
		UE_LOG(LogTemp, Error, TEXT("OnPostSttDevice ########"));
	}

	void OnPreLlmDevice(const FString& InPrompt)
	{
		UE_LOG(LogTemp, Error, TEXT("OnPreLlmDevice ########"));
	}

	void OnPostLlmDevice(const LlmDeviceTask_Result& InResult)
	{
		UE_LOG(LogTemp, Error, TEXT("OnPostLlmDevice ########"));
	}

	void OnPreTtsDevice(const FString& InMessage)
	{
		UE_LOG(LogTemp, Error, TEXT("OnPreTtsDevice ########"));
	}

	void OnPostTtsDevice(const TtsDeviceTask_Result& InResult)
	{
		UE_LOG(LogTemp, Error, TEXT("OnPostTtsDevice ########"));
	}

	void OnPrePlayGameSound(const TArray<uint8>& InAudioBuffer, const UWorld* const InWorld, TSharedPtr<FGlobalContext> GlobalContext)
	{
		UE_LOG(LogTemp, Error, TEXT("OnPrePlayGameSound ########"));
	}

	void OnPostPlayGameSound(
		const TArray<uint8>& InAudioBuffer,
		const UWorld* const InWorld,
		const USoundWaveProcedural* const InSoundWave,
		TSharedPtr<FGlobalContext> GlobalContext)
	{
		UE_LOG(LogTemp, Error, TEXT("OnPostPlayGameSound ########"));
	}

	void OnPreUpdateGameByLlm(const FString& InMessage, const UWorld* const InWorld, TSharedPtr<FGlobalContext> GlobalContext)
	{
		UE_LOG(LogTemp, Error, TEXT("OnPreUpdateGameByLlm ########"));
	}

	void OnPostUpdateGameByLlm(const LlmDeviceTask_Result& InResult, const UWorld* const InWorld, TSharedPtr<FGlobalContext> GlobalContext)
	{
		UE_LOG(LogTemp, Error, TEXT("OnPostUpdateGameByLlm ########"));
	}

public:
	AudioInputTask_Result Audio_Result;
	SttDeviceTask_Result Stt_Result;
	LlmDeviceTask_Result Llm_Result;
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
