// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WhisperUtils.h"
#include "LLMAPITask.h"

class UWorld;

class ConvAITaskGraphExamples
{
public:

	static void AllExamples(UWorld* World);
	static void ConvAITaskGraphExample(UWorld* World);


	static void AudioInputTaskFunction(TArray<uint8>& OutAudioBuffer);
	static void SttApiTaskFunction(const TArray<uint8>& InAudioBuffer, FGraphEventRef FinishEvent, FString& OutSttMessage);
	static void LlmApiTaskFunction(const FString& InMessage, FGraphEventRef FinishEvent, FString& OutLlmMessage);
	static void TtsApiTaskFunction(const FString& InMessage, FGraphEventRef FinishEvent, TArray<uint8>& OutAudioBuffer);
	static void GameTaskFunction(const TArray<uint8>& InAudioBuffer, const UWorld* InWorld);
};


class FFlowContext : public TSharedFromThis<FFlowContext, ESPMode::ThreadSafe>
{
public:
	TArray<uint8> InputAudioBuffer;
	FString SttOutputMessage;
	FString LlmMessage;
	TArray<uint8> TtsAudioBuffer;

};