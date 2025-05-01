#pragma once

#include "CoreMinimal.h"
#include "Commandlets/Commandlet.h"
#include "MainTestCommandlet.generated.h"


// Usage
// UnrealEditor-Cmd.exe ~\UnrealExamples\UnrealExamples.uproject -run=MainTestCommandlet -unattended -nullrhi

// UnrealEditor-Cmd Path
// C:\Program Files\Epic Games\UE_5.5\Engine\Binaries\Win64>UnrealEditor-Cmd.exe

UCLASS()
class UNREALEXAMPLESEDITOR_API UMainTestCommandlet : public UCommandlet
{
    GENERATED_BODY()

public:

    virtual int32 Main(const FString& Params) override;
}; 