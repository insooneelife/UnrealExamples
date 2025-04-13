#include "TTSAPIModule.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "FTTSAPIModule"

void FTTSAPIModule::StartupModule()
{
    UE_LOG(LogTemp, Log, TEXT("TTSAPI module started successfully"));
}

void FTTSAPIModule::ShutdownModule()
{
    UE_LOG(LogTemp, Log, TEXT("TTSAPI module shut down"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FTTSAPIModule, TTSAPI) 