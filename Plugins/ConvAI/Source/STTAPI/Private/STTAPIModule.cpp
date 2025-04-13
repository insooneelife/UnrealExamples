#include "STTAPIModule.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "FSTTAPIModule"

void FSTTAPIModule::StartupModule()
{
    UE_LOG(LogTemp, Log, TEXT("STTAPI module started successfully"));
}

void FSTTAPIModule::ShutdownModule()
{
    UE_LOG(LogTemp, Log, TEXT("STTAPI module shut down"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSTTAPIModule, STTAPI) 