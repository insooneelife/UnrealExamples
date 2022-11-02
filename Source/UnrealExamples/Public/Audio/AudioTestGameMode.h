// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Audio/AudioRecorder.h"
#include "AudioTestGameMode.generated.h"


UCLASS()
class UNREALEXAMPLES_API AAudioTestGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	AAudioTestGameMode();

public:
	virtual void StartPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	void PlayAudioBuffer(const TArray<uint8>& InRawData);

private:

	void OnTimeElapsed();

	void OnAudioCapture(
		const uint8* InBuffer,
		int32 InNum,
		int32 InNumChannels,
		int32 InSampleRate,
		double InStreamTime,
		bool bInOverFlow);

private:

	TSharedPtr<class FAudioRecorder> AudioRecorder;

	FDelegateHandle OnAudioCaptureHandle;
	TArray<uint8> AudioBuffer;
};
