#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FLLMAPIModule : public IModuleInterface
{
public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    /** Get the module instance */
    static inline FLLMAPIModule& Get()
    {
        return FModuleManager::LoadModuleChecked<FLLMAPIModule>("LLMAPI");
    }

    /** Check if the module is loaded */
    static inline bool IsAvailable()
    {
        return FModuleManager::Get().IsModuleLoaded("LLMAPI");
    }
}; 