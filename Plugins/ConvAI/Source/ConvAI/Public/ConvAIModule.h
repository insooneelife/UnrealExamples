#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class CONVAI_API FConvAIModule : public IModuleInterface
{
public:

    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    static inline FConvAIModule& Get()
    {
        return FModuleManager::LoadModuleChecked<FConvAIModule>("ConvAI");
    }

    static inline bool IsAvailable()
    {
        return FModuleManager::Get().IsModuleLoaded("ConvAI");
    }

    static FString GetConfigValue(const FString& Key, const FString& DefaultValue = TEXT(""));
    static void SetConfigValue(const FString& Key, const FString& Value);
    static void LogMessage(const FString& Message, const FString& Category = TEXT("ConvAI"));


	// Cannot use USoundWave, instead use USoundWaveProcedural to create with buffer and play.
    static class USoundWaveProcedural* CreateSoundWaveFromWav(const TArray<uint8>& RawFile);

    static bool LoadWavFileToBuffer(const FString& FilePath, TArray<uint8>& OutBuffer);

    static void LogWithThreadInfo(const FString& Log);

}; 