#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FTTSAPIModule : public IModuleInterface
{
public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    /** Get the module instance */
    static inline FTTSAPIModule& Get()
    {
        return FModuleManager::LoadModuleChecked<FTTSAPIModule>("TTSAPI");
    }

    /** Check if the module is loaded */
    static inline bool IsAvailable()
    {
        return FModuleManager::Get().IsModuleLoaded("TTSAPI");
    }
}; 