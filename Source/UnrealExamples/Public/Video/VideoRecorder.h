// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MediaPlayer.h"
#include "MediaPlayerFacade.h"
#include "IMediaClock.h"
#include "IMediaClockSink.h"
#include "IMediaModule.h"
#include "Video/VideoUtils.h"


class UMediaTexture;
class UMediaPlayer;

DECLARE_STATS_GROUP(TEXT("VideoRecorder"), STATGROUP_VideoRecorder, STATCAT_Advanced);
DECLARE_CYCLE_STAT(TEXT("TickRecording"), STAT_TickRecording, STATGROUP_VideoRecorder);
DECLARE_CYCLE_STAT(TEXT("Convert"), STAT_Convert, STATGROUP_VideoRecorder);
DECLARE_CYCLE_STAT(TEXT("Broadcast"), STAT_Broadcast, STATGROUP_VideoRecorder);

DECLARE_MULTICAST_DELEGATE_FiveParams(FOnVideoCaptureSignature, const uint8*, int32, int32, int32, EMediaTextureSampleFormat);

class FVideoRecorderClockSink;

class UNREALEXAMPLES_API FVideoRecorder : public TSharedFromThis<FVideoRecorder>
{
public:
	FVideoRecorder();

	void StartRecording(UMediaPlayer* InMediaPlayer);
	void StopRecording();

	void TickRecording();

private:

	void SaveImage(TSharedPtr<IMediaTextureSample, ESPMode::ThreadSafe> InSample);

public:
	FOnVideoCaptureSignature OnCapture;

private:
	UPROPERTY()
		UMediaTexture* MediaTexture;

	TSharedPtr<FVideoRecorderClockSink, ESPMode::ThreadSafe> ClockSink;

	TSharedPtr<FMediaTextureSampleQueue, ESPMode::ThreadSafe> SampleQueue;

	TArray<uint8> Stream;

	int32 FrameCount;

	bool bRecording;
};


class FVideoRecorderClockSink
	: public IMediaClockSink
{
public:

	FVideoRecorderClockSink(FVideoRecorder& InOwner)
		: Owner(InOwner)
	{ }

	virtual ~FVideoRecorderClockSink() { }

public:

	virtual void TickOutput(FTimespan DeltaTime, FTimespan Timecode) override
	{
		Owner.TickRecording();
	}

private:

	FVideoRecorder& Owner;
};