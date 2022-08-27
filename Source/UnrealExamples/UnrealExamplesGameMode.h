// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "UnrealExamplesGameMode.generated.h"

DECLARE_STATS_GROUP(TEXT("UnrealExamples"), STATGROUP_UnrealExamples, STATCAT_Advanced);
DECLARE_CYCLE_STAT(TEXT("Tick"), STAT_Tick, STATGROUP_UnrealExamples);

UCLASS()
class UNREALEXAMPLES_API AUnrealExamplesGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:

	virtual void StartPlay() override;

	virtual void Tick(float DeltaSeconds) override;

private:
};
