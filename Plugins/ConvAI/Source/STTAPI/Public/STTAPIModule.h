#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FSTTAPIModule : public IModuleInterface
{
public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    /** Get the module instance */
    static inline FSTTAPIModule& Get()
    {
        return FModuleManager::LoadModuleChecked<FSTTAPIModule>("STTAPI");
    }

    /** Check if the module is loaded */
    static inline bool IsAvailable()
    {
        return FModuleManager::Get().IsModuleLoaded("STTAPI");
    }
}; 