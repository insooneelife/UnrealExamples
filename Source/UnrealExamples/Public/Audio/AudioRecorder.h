#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#if PLATFORM_WINDOWS
#include "Windows/WindowsHWrapper.h"
#endif
THIRD_PARTY_INCLUDES_START
#include "RtAudio.h"
THIRD_PARTY_INCLUDES_END

#include "Audio/AudioUtils.h"

DECLARE_MULTICAST_DELEGATE_SixParams(FOnCaptureSignature, const uint8*, int32, int32, int32, double, bool);




class UNREALEXAMPLES_API FAudioRecorder : public TSharedFromThis<FAudioRecorder>
{
public:
	FAudioRecorder();

	~FAudioRecorder();

public:

	bool OpenStream();

	bool CloseStream();

	bool StartStream();

	bool StopStream();

	void OnAudioCapture(void* InBuffer, uint32 InBufferFrames, double StreamTime, bool bOverflow);

private:
	bool OpenCaptureStream();

public:
	static void ConvertSampleRate(
		float CurrentSR,
		float TargetSR,
		int32 NumChannels,
		const TArray<int16>& CurrentRecordedPCMData,
		int32 NumSamplesToConvert,
		TArray<int16>& OutConverted);

public:
	FOnCaptureSignature OnCapture;

private:
	RtAudio CaptureDevice;
	RtAudio::StreamParameters StreamParams;


	int32 NumChannels;
	int32 SampleRate;
	FCriticalSection CaptureCriticalSection;
};