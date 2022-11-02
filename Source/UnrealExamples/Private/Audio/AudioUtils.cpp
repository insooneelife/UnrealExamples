#include "Audio/AudioUtils.h"
#include "Sound/SoundWaveProcedural.h"

FAudioData AudioUtils::AudioData;

void AudioUtils::PCMToWaveFile(
	TArray<uint8>& OutWaveFileData, const uint8* InPCMData, const int32 NumBytes, const int32 NumChannels, const int32 SampleRate)
{
	SerializeWaveFile(OutWaveFileData, InPCMData, NumBytes, NumChannels, SampleRate);
}

USoundWave* AudioUtils::RawWaveToSoundWave(
	const uint8* InWaveData, int32 InWaveDataSize, uint32 InSampleRate, uint16 InNumChannels)
{
	USoundWaveProcedural* Sound = NewObject<USoundWaveProcedural>(USoundWaveProcedural::StaticClass());
	if (Sound)
	{
		Sound->SetSampleRate(InSampleRate);
		Sound->NumChannels = InNumChannels;

		static const int32 BIT_RATE = 16;
		int32 DurationDiv = Sound->NumChannels * BIT_RATE * Sound->GetSampleRateForCurrentPlatform();
		if (DurationDiv)
		{
			Sound->Duration = InWaveDataSize * 8.0f / DurationDiv;
		}
		else
		{
			Sound->Duration = 0.0f;
		}

		Sound->RawPCMDataSize = InWaveDataSize;
		Sound->SoundGroup = SOUNDGROUP_Default;
		Sound->QueueAudio(InWaveData, InWaveDataSize);
	}

	return Sound;
}