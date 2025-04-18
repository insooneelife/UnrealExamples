#include "LLMAPIModule.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "FLLMAPIModule"

void FLLMAPIModule::StartupModule()
{
    UE_LOG(LogTemp, Log, TEXT("LLMAPI module started successfully"));
}

void FLLMAPIModule::ShutdownModule()
{
    UE_LOG(LogTemp, Log, TEXT("LLMAPI module shut down"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FLLMAPIModule, LLMAPI) 