// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class USoundWave;

struct UNREALEXAMPLES_API FAudioData
{
public:
	FAudioData()
		:
		Channel(1), BitRate(16000), SampleWidth(2), Duration(30), Timestamp(0)
	{}

	int32 GetSampleSize() const { return BitRate * SampleWidth * Duration / 1000; }

public:
	uint8 Channel;
	uint16 BitRate;
	uint8 SampleWidth;
	uint16 Duration;
	uint64 Timestamp;
};


class UNREALEXAMPLES_API AudioUtils
{
public:
	static FAudioData AudioData;

public:
	static void PCMToWaveFile(
		TArray<uint8>& OutWaveFileData, const uint8* InPCMData, const int32 NumBytes, const int32 NumChannels, const int32 SampleRate);

	static USoundWave* RawWaveToSoundWave(
		const uint8* InWaveData, int32 InWaveDataSize, uint32 InSampleRate, uint16 InNumChannels); 

	static void SerializeWaveFile(
		TArray<uint8>& OutWaveFileData,
		const uint8* InPCMData,
		int32 NumBytes,
		int32 NumChannels,
		int32 SampleRate,
		int32 BitsPerSample = 16);
};
