// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WhisperUtils.h"
#include "LLMAPITask.h"

class UWorld;


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

public:
	AudioInputTask_Result Audio_Result;
	SttDeviceTask_Result Stt_Result;
	LlmDeviceTask_Result Llm_Result;
	TtsDeviceTask_Result Tts_Result;
};


class ConvAITaskGraphExamples
{
public:

	static void AllExamples(UWorld* World);
	static void ApiFlowTaskGraphExample(UWorld* World);
	static void DeviceFlowTaskGraphExample(UWorld* World);


	static void AudioInputTaskFunction(AudioInputTask_Result& OutResult);
	static void SttApiTaskFunction(const TArray<uint8>& InAudioBuffer, FGraphEventRef FinishEvent, SttApiTask_Result& OutResult);
	static void LlmApiTaskFunction(const FString& InMessage, FGraphEventRef FinishEvent, LlmApiTask_Result& OutResult);
	static void TtsApiTaskFunction(const FString& InMessage, FGraphEventRef FinishEvent, TtsApiTask_Result& OutResult);

	static void SttDeviceTaskFunction(const TArray<uint8>& InAudioBuffer, SttDeviceTask_Result& OutResult);
	static void LlmDeviceTaskFunction(const FString& InMessage, LlmDeviceTask_Result& OutResult);
	static void TtsDeviceTaskFunction(const FString& InMessage, TtsDeviceTask_Result& OutResult);

	static void GameTaskFunction(const TArray<uint8>& InAudioBuffer, const UWorld* InWorld);
};


