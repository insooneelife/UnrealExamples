// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WhisperUtils.h"
#include "LLMAPITask.h"

class UWorld;


struct AudioInputTask_Result
{
	bool bSuccess;
	TArray<uint8> InputAudioBuffer;
};

struct SttApiTask_Result
{
	bool bSuccess;
	FString SttMessage;
};

struct LlmApiTask_Result
{
	bool bSuccess;
	FString LlmMessage;
};

struct TtsApiTask_Result
{
	bool bSuccess;
	TArray<uint8> TtsAudioBuffer;
};

class FFlowContext : public TSharedFromThis<FFlowContext, ESPMode::ThreadSafe>
{
public:
	AudioInputTask_Result AudioInput_Result;
	SttApiTask_Result SttApi_Result;
	LlmApiTask_Result LlmApi_Result;
	TtsApiTask_Result TtsApi_Result;
};


class ConvAITaskGraphExamples
{
public:

	static void AllExamples(UWorld* World);
	static void ConvAITaskGraphExample(UWorld* World);


	static void AudioInputTaskFunction(AudioInputTask_Result& OutResult);
	static void SttApiTaskFunction(const TArray<uint8>& InAudioBuffer, FGraphEventRef FinishEvent, SttApiTask_Result& OutResult);
	static void LlmApiTaskFunction(const FString& InMessage, FGraphEventRef FinishEvent, LlmApiTask_Result& OutResult);
	static void TtsApiTaskFunction(const FString& InMessage, FGraphEventRef FinishEvent, TtsApiTask_Result& OutResult);
	static void GameTaskFunction(const TArray<uint8>& InAudioBuffer, const UWorld* InWorld);
};


