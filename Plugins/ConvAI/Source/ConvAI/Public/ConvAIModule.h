#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class CONVAI_API FConvAIModule : public IModuleInterface
{
public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    /** Get the module instance */
    static inline FConvAIModule& Get()
    {
        return FModuleManager::LoadModuleChecked<FConvAIModule>("ConvAI");
    }

    /** Check if the module is loaded */
    static inline bool IsAvailable()
    {
        return FModuleManager::Get().IsModuleLoaded("ConvAI");
    }

    /** Common functionality that can be used by other modules */
    static FString GetConfigValue(const FString& Key, const FString& DefaultValue = TEXT(""));
    static void SetConfigValue(const FString& Key, const FString& Value);
    static void LogMessage(const FString& Message, const FString& Category = TEXT("ConvAI"));

    class USoundWave* CreateSoundWaveFromWav(const TArray<uint8>& RawFile);

    bool LoadWavFileToBuffer(const FString& FilePath, TArray<uint8>& OutBuffer);

    void LogWithThreadInfo(const FString& Log);

}; 