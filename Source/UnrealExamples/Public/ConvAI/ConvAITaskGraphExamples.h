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

// Delegate declarations
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBeginDelegate, TSharedPtr<FGlobalContext>);
DECLARE_MULTICAST_DELEGATE(FOnEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnPreCollectAudioInputDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPostCollectAudioInputDelegate, const AudioInputTask_Result&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPreSttApiDelegate, const TArray<uint8>&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPostSttApiDelegate, const SttApiTask_Result&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPreLlmApiDelegate, const FString&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPostLlmApiDelegate, const LlmApiTask_Result&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPreTtsApiDelegate, const FString&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPostTtsApiDelegate, const TtsApiTask_Result&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnPrePlayGameSoundDelegate, const TArray<uint8>&, const UWorld* const, TSharedPtr<FGlobalContext>);
DECLARE_MULTICAST_DELEGATE_FourParams(FOnPostPlayGameSoundDelegate, const TArray<uint8>&, const UWorld* const, const USoundWaveProcedural* const, TSharedPtr<FGlobalContext>);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPreSttDeviceDelegate, const TArray<uint8>&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPostSttDeviceDelegate, const SttDeviceTask_Result&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPreLlmDeviceDelegate, const FString&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPostLlmDeviceDelegate, const LlmDeviceTask_Result&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnPreUpdateGameByLlmDelegate, const FString&, const UWorld* const, TSharedPtr<FGlobalContext>);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnPostUpdateGameByLlmDelegate, const LlmDeviceTask_Result&, const UWorld* const, TSharedPtr<FGlobalContext>);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPreTtsDeviceDelegate, const FString&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPostTtsDeviceDelegate, const TtsDeviceTask_Result&);

class FApiFlowContext : public TSharedFromThis<FApiFlowContext, ESPMode::ThreadSafe>
{
public:
	// Delegate instances
	FOnBeginDelegate OnBeginDelegate;
	FOnEndDelegate OnEndDelegate;
	FOnPreCollectAudioInputDelegate OnPreCollectAudioInputDelegate;
	FOnPostCollectAudioInputDelegate OnPostCollectAudioInputDelegate;
	FOnPreSttApiDelegate OnPreSttApiDelegate;
	FOnPostSttApiDelegate OnPostSttApiDelegate;
	FOnPreLlmApiDelegate OnPreLlmApiDelegate;
	FOnPostLlmApiDelegate OnPostLlmApiDelegate;
	FOnPreTtsApiDelegate OnPreTtsApiDelegate;
	FOnPostTtsApiDelegate OnPostTtsApiDelegate;
	FOnPrePlayGameSoundDelegate OnPrePlayGameSoundDelegate;
	FOnPostPlayGameSoundDelegate OnPostPlayGameSoundDelegate;

	void Reset()
	{
		Audio_Result.Reset();
		Stt_Result.Reset();
		Llm_Result.Reset();
		Tts_Result.Reset();
	}

	void OnBegin(TSharedPtr<FGlobalContext> GlobalContext)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FApiFlowContext::OnBegin]"));
		OnBeginDelegate.Broadcast(GlobalContext);
	}

	void OnEnd(TSharedPtr<FGlobalContext> GlobalContext)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FApiFlowContext::OnEnd]"));
		OnEndDelegate.Broadcast();
	}

	void OnPreCollectAudioInput()
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FApiFlowContext::OnPreCollectAudioInput]"));
		OnPreCollectAudioInputDelegate.Broadcast();
	}

	void OnPostCollectAudioInput(const AudioInputTask_Result& InResult)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FApiFlowContext::OnPostCollectAudioInput]"));
		OnPostCollectAudioInputDelegate.Broadcast(InResult);
	}
	
	void OnPreSttApi(const TArray<uint8>& InAudioBuffer)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FApiFlowContext::OnPreSttApi]"));
		OnPreSttApiDelegate.Broadcast(InAudioBuffer);
	}

	void OnPostSttApi(const SttApiTask_Result& InResult)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FApiFlowContext::OnPostSttApi]"));

		if(InResult.bSuccess)
		{
			FString LogMsg = FString::Printf(TEXT("[FApiFlowContext::OnPostSttApi] Stt Response : %s"), *InResult.Message);
			FConvAIModule::LogWithThreadInfo(LogMsg);
		}
		OnPostSttApiDelegate.Broadcast(InResult);
	}

	void OnPreLlmApi(const FString& InPrompt)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FApiFlowContext::OnPreLlmApi]"));
		OnPreLlmApiDelegate.Broadcast(InPrompt);
	}

	void OnPostLlmApi(const LlmApiTask_Result& InResult)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FApiFlowContext::OnPostLlmApi]"));

		if(InResult.bSuccess)
		{
			FString LogMsg = FString::Printf(TEXT("[FApiFlowContext::OnPostLlmApi] Llm Response. : %s"), *InResult.Message);
			FConvAIModule::LogWithThreadInfo(LogMsg);
		}
		OnPostLlmApiDelegate.Broadcast(InResult);
	}

	void OnPreTtsApi(const FString& InMessage)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FApiFlowContext::OnPreTtsApi]"));
		OnPreTtsApiDelegate.Broadcast(InMessage);
	}

	void OnPostTtsApi(const TtsApiTask_Result& InResult)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FApiFlowContext::OnPostTtsApi]"));

		if(InResult.bSuccess)
		{
			FString LogMsg = FString::Printf(TEXT("[FApiFlowContext::OnPostTtsApi] Tts Response : %d"), InResult.AudioBuffer.Num());
			FConvAIModule::LogWithThreadInfo(LogMsg);
		}
		OnPostTtsApiDelegate.Broadcast(InResult);
	}

	// needs to check if world is valid
	void OnPrePlayGameSound(const TArray<uint8>& InAudioBuffer, const UWorld* const InWorld, TSharedPtr<FGlobalContext> GlobalContext)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FApiFlowContext::OnPrePlayGameSound]"));
		OnPrePlayGameSoundDelegate.Broadcast(InAudioBuffer, InWorld, GlobalContext);
	}

	void OnPostPlayGameSound(
		const TArray<uint8>& InAudioBuffer, 
		const UWorld* const InWorld, 
		const USoundWaveProcedural* const InSoundWave, 
		TSharedPtr<FGlobalContext> GlobalContext)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FApiFlowContext::OnPostPlayGameSound]"));
		if (IsValid(InSoundWave))
		{
			FConvAIModule::LogWithThreadInfo(TEXT("[FApiFlowContext::OnPostPlayGameSound] Sound Wave Successfully Created."));
		}
		OnPostPlayGameSoundDelegate.Broadcast(InAudioBuffer, InWorld, InSoundWave, GlobalContext);
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
	// Delegate instances
	FOnBeginDelegate OnBeginDelegate;
	FOnEndDelegate OnEndDelegate;
	FOnPreCollectAudioInputDelegate OnPreCollectAudioInputDelegate;
	FOnPostCollectAudioInputDelegate OnPostCollectAudioInputDelegate;
	FOnPreSttDeviceDelegate OnPreSttDeviceDelegate;
	FOnPostSttDeviceDelegate OnPostSttDeviceDelegate;
	FOnPreLlmDeviceDelegate OnPreLlmDeviceDelegate;
	FOnPostLlmDeviceDelegate OnPostLlmDeviceDelegate;
	FOnPreUpdateGameByLlmDelegate OnPreUpdateGameByLlmDelegate;
	FOnPostUpdateGameByLlmDelegate OnPostUpdateGameByLlmDelegate;

	void Reset()
	{
		Audio_Result.Reset();
		Stt_Result.Reset();
		Llm_Result.Reset();
	}

	void OnBegin(TSharedPtr<FGlobalContext> GlobalContext)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FDeviceFlowContext::OnBegin]"));
		OnBeginDelegate.Broadcast(GlobalContext);
	}

	void OnEnd(TSharedPtr<FGlobalContext> GlobalContext)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FDeviceFlowContext::OnEnd]"));
		OnEndDelegate.Broadcast();
	}

	void OnPreCollectAudioInput()
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FApiFlowContext::OnPreCollectAudioInput]"));
		OnPreCollectAudioInputDelegate.Broadcast();
	}

	void OnPostCollectAudioInput(const AudioInputTask_Result& InResult)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FApiFlowContext::OnPostCollectAudioInput]"));
		OnPostCollectAudioInputDelegate.Broadcast(InResult);
	}


	void OnPreSttDevice(const TArray<uint8>& InAudioBuffer)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FDeviceFlowContext::OnPreSttDevice]"));
		OnPreSttDeviceDelegate.Broadcast(InAudioBuffer);
	}

	void OnPostSttDevice(const SttDeviceTask_Result& InResult)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FDeviceFlowContext::OnPostSttDevice]"));
		OnPostSttDeviceDelegate.Broadcast(InResult);
	}

	void OnPreLlmDevice(const FString& InPrompt)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FDeviceFlowContext::OnPreLlmDevice]"));
		OnPreLlmDeviceDelegate.Broadcast(InPrompt);
	}

	void OnPostLlmDevice(const LlmDeviceTask_Result& InResult)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FDeviceFlowContext::OnPostLlmDevice]"));
		OnPostLlmDeviceDelegate.Broadcast(InResult);
	}

	void OnPreUpdateGameByLlm(const FString& InMessage, const UWorld* const InWorld, TSharedPtr<FGlobalContext> GlobalContext)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FDeviceFlowContext::OnPreUpdateGameByLlm]"));
		OnPreUpdateGameByLlmDelegate.Broadcast(InMessage, InWorld, GlobalContext);
	}

	void OnPostUpdateGameByLlm(const LlmDeviceTask_Result& InResult, const UWorld* const InWorld, TSharedPtr<FGlobalContext> GlobalContext)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FDeviceFlowContext::OnPostUpdateGameByLlm]"));
		OnPostUpdateGameByLlmDelegate.Broadcast(InResult, InWorld, GlobalContext);
	}

public:
	AudioInputTask_Result Audio_Result;
	SttDeviceTask_Result Stt_Result;
	LlmDeviceTask_Result Llm_Result;
};

class FDevicePlaySoundFlowContext : public TSharedFromThis<FDevicePlaySoundFlowContext, ESPMode::ThreadSafe>
{
public:
	// Delegate instances
	FOnBeginDelegate OnBeginDelegate;
	FOnEndDelegate OnEndDelegate;
	FOnPreTtsDeviceDelegate OnPreTtsDeviceDelegate;
	FOnPostTtsDeviceDelegate OnPostTtsDeviceDelegate;
	FOnPrePlayGameSoundDelegate OnPrePlayGameSoundDelegate;
	FOnPostPlayGameSoundDelegate OnPostPlayGameSoundDelegate;

	void Reset()
	{
		LlmMessage.Reset();
		Tts_Result.Reset();
	}

	void OnBegin(TSharedPtr<FGlobalContext> GlobalContext)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FDevicePlaySoundFlowContext::OnBegin]"));
		OnBeginDelegate.Broadcast(GlobalContext);
	}

	void OnEnd(TSharedPtr<FGlobalContext> GlobalContext)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FDevicePlaySoundFlowContext::OnEnd]"));
		OnEndDelegate.Broadcast();
	}

	void OnPreTtsDevice(const FString& InMessage)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FDevicePlaySoundFlowContext::OnPreTtsDevice]"));
		OnPreTtsDeviceDelegate.Broadcast(InMessage);
	}

	void OnPostTtsDevice(const TtsDeviceTask_Result& InResult)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FDevicePlaySoundFlowContext::OnPostTtsDevice]"));

		if (InResult.bSuccess)
		{
			FString LogMsg = FString::Printf(TEXT("[FDevicePlaySoundFlowContext::OnPostTtsDevice] Tts Response : %d"), InResult.AudioBuffer.Num());
			FConvAIModule::LogWithThreadInfo(LogMsg);
		}
		OnPostTtsDeviceDelegate.Broadcast(InResult);
	}

	void OnPrePlayGameSound(const TArray<uint8>& InAudioBuffer, const UWorld* const InWorld, TSharedPtr<FGlobalContext> GlobalContext)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FDevicePlaySoundFlowContext::OnPrePlayGameSound]"));
		OnPrePlayGameSoundDelegate.Broadcast(InAudioBuffer, InWorld, GlobalContext);
	}

	void OnPostPlayGameSound(
		const TArray<uint8>& InAudioBuffer,
		const UWorld* const InWorld,
		const USoundWaveProcedural* const InSoundWave,
		TSharedPtr<FGlobalContext> GlobalContext)
	{
		FConvAIModule::LogWithThreadInfo(TEXT("[FDevicePlaySoundFlowContext::OnPostPlayGameSound]"));
		if (IsValid(InSoundWave))
		{
			FConvAIModule::LogWithThreadInfo(TEXT("[FDevicePlaySoundFlowContext::OnPostPlayGameSound] Sound Wave Successfully Created."));
		}
		OnPostPlayGameSoundDelegate.Broadcast(InAudioBuffer, InWorld, InSoundWave, GlobalContext);
	}
public:
	FString LlmMessage;
	TtsDeviceTask_Result Tts_Result;
};

class UNREALEXAMPLES_API ConvAITaskGraphExamples
{
public:

	static void AllExamples(UWorld* const World);
	static void TriggerApiTaskFlow(TSharedPtr<FGlobalContext> GlobalContext, UWorld* const World);
	static void TriggerDeviceProducerTaskFlow(TSharedPtr<FGlobalContext> GlobalContext, UWorld* const World);
	static void TriggerDeviceConsumerTaskFlow(TSharedPtr<FGlobalContext> GlobalContext, UWorld* const World);

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
