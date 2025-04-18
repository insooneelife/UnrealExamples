#include "ConvAIModule.h"
#include "Modules/ModuleManager.h"
#include "Misc/ConfigCacheIni.h"

#include "Sound/SoundWave.h"
#include "Sound/SoundWaveProcedural.h"
#include "Audio.h"

#include "Serialization/BufferArchive.h"
#include "Misc/Base64.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "HAL/ThreadManager.h"


#define LOCTEXT_NAMESPACE "FConvAIModule"

void FConvAIModule::StartupModule()
{
    UE_LOG(LogTemp, Log, TEXT("ConvAI module started successfully"));
}

void FConvAIModule::ShutdownModule()
{
    UE_LOG(LogTemp, Log, TEXT("ConvAI module shut down"));
}

FString FConvAIModule::GetConfigValue(const FString& Key, const FString& DefaultValue)
{
    FString Value;
    if (GConfig->GetString(TEXT("ConvAI"), *Key, Value, GEngineIni))
    {
        return Value;
    }
    return DefaultValue;
}

void FConvAIModule::SetConfigValue(const FString& Key, const FString& Value)
{
    GConfig->SetString(TEXT("ConvAI"), *Key, *Value, GEngineIni);
    GConfig->Flush(false, GEngineIni);
}

void FConvAIModule::LogMessage(const FString& Message, const FString& Category)
{
    UE_LOG(LogTemp, Log, TEXT("[%s] %s"), *Category, *Message);
}

USoundWaveProcedural* FConvAIModule::CreateSoundWaveFromWav(const TArray<uint8>& RawFile)
{
	FWaveModInfo WaveInfo;
	if (!WaveInfo.ReadWaveInfo(const_cast<uint8*>(RawFile.GetData()), RawFile.Num()))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid WAV format"));
		return nullptr;
	}

	const int32 NumChannels = *WaveInfo.pChannels;
	const int32 SampleRate = *WaveInfo.pSamplesPerSec;
	const int32 BitsPerSample = *WaveInfo.pBitsPerSample;
	const int32 SampleDataSize = WaveInfo.SampleDataSize;

	if (BitsPerSample != 16)
	{
		UE_LOG(LogTemp, Error, TEXT("Only 16-bit PCM is supported."));
		return nullptr;
	}

	USoundWaveProcedural* SoundWave = NewObject<USoundWaveProcedural>();
	SoundWave->SetSampleRate(SampleRate);
	SoundWave->NumChannels = NumChannels;
	SoundWave->Duration = (float)SampleDataSize / (NumChannels * (BitsPerSample / 8) * SampleRate);
	SoundWave->SoundGroup = SOUNDGROUP_Default;
	SoundWave->bLooping = false;

	TArray<uint8> PCMData;
	PCMData.Append(WaveInfo.SampleDataStart, SampleDataSize);
	SoundWave->QueueAudio(PCMData.GetData(), PCMData.Num());

	return SoundWave;
}




bool FConvAIModule::LoadWavFileToBuffer(const FString& FilePath, TArray<uint8>& OutBuffer)
{
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("wave file not found: %s"), *FilePath);
		return false;
	}

	if (!FFileHelper::LoadFileToArray(OutBuffer, *FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("wave file read failed: %s"), *FilePath);
		return false;
	}

	UE_LOG(LogTemp, Log, TEXT("wave file load completed (%d bytes): %s"), OutBuffer.Num(), *FilePath);
	return true;
}

void FConvAIModule::LogWithThreadInfo(const FString& Log)
{
	uint32 ThreadId = FPlatformTLS::GetCurrentThreadId();
	FString ThreadName = FThreadManager::Get().GetThreadName(ThreadId);
	UE_LOG(LogTemp, Log, TEXT("ThreadId: %d  ThreadName: %s\n"), ThreadId, *ThreadName, *Log);
}



#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FConvAIModule, ConvAI) 