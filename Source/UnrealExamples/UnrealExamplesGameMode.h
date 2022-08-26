// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Networking/TCPSocketClientExamples.h"
#include "UnrealExamplesGameMode.generated.h"

/**
 * 
 */
UCLASS()
class UNREALEXAMPLES_API AUnrealExamplesGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:

	virtual void StartPlay() override;

	virtual void Tick(float DeltaSeconds) override;

private:

	TCPSocketClientExamples Ex;
};
