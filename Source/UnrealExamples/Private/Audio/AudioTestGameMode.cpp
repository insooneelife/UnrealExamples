// Fill out your copyright notice in the Description page of Project Settings.


#include "Audio/AudioTestGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundWaveProcedural.h"


AAudioTestGameMode::AAudioTestGameMode()
{
	AudioRecorder = MakeShareable(new FAudioRecorder());
}


void AAudioTestGameMode::StartPlay()
{
	Super::StartPlay();
	AudioRecorder->OpenStream();
	AudioRecorder->StartStream();

	OnAudioCaptureHandle = AudioRecorder->OnCapture.AddUObject(this, &AAudioTestGameMode::OnAudioCapture);

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AAudioTestGameMode::OnTimeElapsed, 4.0f, false);
}



void AAudioTestGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	AudioRecorder->CloseStream();

	AudioRecorder->OnCapture.Remove(OnAudioCaptureHandle);

	Super::EndPlay(EndPlayReason);
}


void AAudioTestGameMode::PlayAudioBuffer(const TArray<uint8>& InRawData)
{
	UE_LOG(LogTemp, Error, TEXT("PlayAudioBuffer  Num : %d"), InRawData.Num());


	TArray<uint8> WaveFileData;
	AudioUtils::PCMToWaveFile(
		WaveFileData,
		InRawData.GetData(),
		InRawData.Num(),
		1,
		16000);

	//UE_LOG(LogTemp, Error, TEXT("BeginIndex : %d  EndIndex : %d  BufferBeginIndex : %d   BufferEndIndex : %d  WaveFileData : %d"),
	//	VAD.BeginIndex, VAD.EndIndex, BufferBeginIndex, BufferEndIndex, WaveFileData.Num());

	USoundWave* SoundWave = AudioUtils::RawWaveToSoundWave(
		WaveFileData.GetData(),
		WaveFileData.Num(),
		1,
		16000);

	UGameplayStatics::PlaySound2D(GetWorld(), SoundWave);

}

void AAudioTestGameMode::OnTimeElapsed()
{
	PlayAudioBuffer(AudioBuffer);

	AudioBuffer.Empty();
}

void AAudioTestGameMode::OnAudioCapture(
	const uint8* InBuffer,
	int32 InNum,
	int32 InNumChannels,
	int32 InSampleRate,
	double InStreamTime,
	bool bInOverFlow)
{
	AudioBuffer.Append(InBuffer, InNum);

	if (AudioBuffer.Num() > InNum * 100)
	{
		AudioBuffer.Empty(AudioBuffer.Num());
	}
}