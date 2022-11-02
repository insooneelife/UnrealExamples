// Fill out your copyright notice in the Description page of Project Settings.


#include "Audio/AudioRecorder.h"
#include "AudioCaptureCore.h"

static int32 OnAudioCaptureCallback(
	void* OutBuffer, void* InBuffer, uint32 InBufferFrames, double StreamTime, RtAudioStreamStatus AudioStreamStatus, void* InUserData)
{
	FAudioRecorder* AudioRecorder = (FAudioRecorder*)InUserData;
	AudioRecorder->OnAudioCapture(InBuffer, InBufferFrames, StreamTime, AudioStreamStatus == RTAUDIO_INPUT_OVERFLOW);
	return 0;
}

FAudioRecorder::FAudioRecorder()
{
	NumChannels = 1;
	SampleRate = 16000;
}

FAudioRecorder::~FAudioRecorder()
{
	if (CaptureDevice.isStreamOpen())
	{
		CaptureDevice.abortStream();
	}
}

bool FAudioRecorder::OpenStream()
{
	return OpenCaptureStream();
}

bool FAudioRecorder::OpenCaptureStream()
{
	uint32 NumFramesDesired = 960;
	uint32 InputDeviceId = CaptureDevice.getDefaultInputDevice();

	RtAudio::DeviceInfo DeviceInfo = CaptureDevice.getDeviceInfo(InputDeviceId);

	RtAudio::StreamParameters RtAudioStreamParams;
	RtAudioStreamParams.deviceId = InputDeviceId;
	RtAudioStreamParams.firstChannel = 0;
	RtAudioStreamParams.nChannels = NumChannels;

	if (CaptureDevice.isStreamOpen())
	{
		CaptureDevice.stopStream();
		CaptureDevice.closeStream();
	}

	uint32 NumFrames = NumFramesDesired;
	RtAudioFormat Formats = RTAUDIO_SINT16;

	CaptureDevice.openStream(nullptr, &RtAudioStreamParams, Formats, SampleRate, &NumFrames, &OnAudioCaptureCallback, this);

	if (!CaptureDevice.isStreamOpen())
	{
		UE_LOG(LogTemp, Error, TEXT("OpenStream Failed."));
		return false;
	}

	UE_LOG(LogTemp, Log, TEXT("OpenStream Success.  NumChannels : %d  SampleRate : %d  Formats : %d"),
		NumChannels,
		SampleRate,
		Formats);

	return true;
}

void FAudioRecorder::OnAudioCapture(void* InBuffer, uint32 InBufferFrames, double StreamTime, bool bOverflow)
{
	int16* InBufferData = (int16*)InBuffer;

	auto OnCaptureLambda = [this](
		const int16* InAudioData, int32 NumFrames, int32 NumChannels, int32 SampleRate, double StreamTime, bool bOverFlow)
	{
		int32 NumSamples = NumChannels * NumFrames;

		FScopeLock Lock(&CaptureCriticalSection);

		OnCapture.Broadcast((uint8*)InAudioData, NumSamples * sizeof(int16), NumChannels, SampleRate, StreamTime, bOverFlow);

		//UE_LOG(LogTemp, Log, TEXT("NumChannels : %d  NumSamples : %d  AudioBuffer Num : %d  SampleRate : %d"),
		//	NumChannels, NumSamples, AudioBuffer.Num(), SampleRate);
	};

	OnCaptureLambda(InBufferData, InBufferFrames, NumChannels, SampleRate, StreamTime, bOverflow);
}

bool FAudioRecorder::CloseStream()
{
	if (CaptureDevice.isStreamOpen())
	{
		CaptureDevice.closeStream();
	}
	return true;
}

bool FAudioRecorder::StartStream()
{
	CaptureDevice.startStream();
	return true;
}

bool FAudioRecorder::StopStream()
{
	if (CaptureDevice.isStreamOpen())
	{
		CaptureDevice.stopStream();
	}
	return true;
}


void FAudioRecorder::ConvertSampleRate(
	float CurrentSR,
	float TargetSR,
	int32 NumChannels,
	const TArray<int16>& CurrentRecordedPCMData,
	int32 NumSamplesToConvert,
	TArray<int16>& OutConverted)
{
	int32 NumInputSamples = CurrentRecordedPCMData.Num();
	int32 NumOutputSamples = NumInputSamples * TargetSR / CurrentSR;

	OutConverted.Reset(NumOutputSamples);

	float SrFactor = (double)CurrentSR / TargetSR;
	float CurrentFrameIndexInterpolated = 0.0f;
	check(NumSamplesToConvert <= CurrentRecordedPCMData.Num());
	int32 NumFramesToConvert = NumSamplesToConvert / NumChannels;
	int32 CurrentFrameIndex = 0;

	for (;;)
	{
		int32 NextFrameIndex = CurrentFrameIndex + 1;
		if (CurrentFrameIndex >= NumFramesToConvert || NextFrameIndex >= NumFramesToConvert)
		{
			break;
		}

		for (int32 Channel = 0; Channel < NumChannels; ++Channel)
		{
			int32 CurrentSampleIndex = CurrentFrameIndex * NumChannels + Channel;
			int32 NextSampleIndex = CurrentSampleIndex + NumChannels;

			int16 CurrentSampleValue = CurrentRecordedPCMData[CurrentSampleIndex];
			int16 NextSampleValue = CurrentRecordedPCMData[NextSampleIndex];

			int16 NewSampleValue = FMath::Lerp(CurrentSampleValue, NextSampleValue, CurrentFrameIndexInterpolated);

			OutConverted.Add(NewSampleValue);
		}

		CurrentFrameIndexInterpolated += SrFactor;

		// Wrap the interpolated frame between 0.0 and 1.0 to maintain float precision
		while (CurrentFrameIndexInterpolated >= 1.0f)
		{
			CurrentFrameIndexInterpolated -= 1.0f;

			// Every time it wraps, we increment the frame index
			++CurrentFrameIndex;
		}
	}
}
