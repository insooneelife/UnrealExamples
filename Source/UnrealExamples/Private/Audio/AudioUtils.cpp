#include "Audio/AudioUtils.h"
#include "Sound/SoundWaveProcedural.h"
#include "Serialization/ArrayWriter.h"

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

void AudioUtils::SerializeWaveFile(
	TArray<uint8>& OutWaveFileData,
	const uint8* InPCMData,
	int32 NumBytes,
	int32 NumChannels,
	int32 SampleRate,
	int32 BitsPerSample)
{
	OutWaveFileData.Reset();
	FArrayWriter Ar;
	int32 ByteRate = SampleRate * NumChannels * BitsPerSample / 8;
	int32 BlockAlign = NumChannels * BitsPerSample / 8;
	int32 DataSize = NumBytes;
	int32 ChunkSize = 36 + DataSize;

	// RIFF Header
	Ar.Serialize((void*)"RIFF", 4);
	Ar << ChunkSize;
	Ar.Serialize((void*)"WAVE", 4);

	// fmt subchunk
	Ar.Serialize((void*)"fmt ", 4);
	int32 Subchunk1Size = 16;
	int16 AudioFormat = 1; // PCM
	Ar << Subchunk1Size;
	Ar << AudioFormat;
	Ar << NumChannels;
	Ar << SampleRate;
	Ar << ByteRate;
	Ar << BlockAlign;
	Ar << BitsPerSample;

	// data subchunk
	Ar.Serialize((void*)"data", 4);
	Ar << DataSize;

	// PCM data
	Ar.Serialize((void*)InPCMData, NumBytes);

	// Copy to output
	OutWaveFileData = Ar;
	
}