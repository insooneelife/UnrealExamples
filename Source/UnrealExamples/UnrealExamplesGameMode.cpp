// Copyright Epic Games, Inc. All Rights Reserved.


#include "UnrealExamplesGameMode.h"
#include "Serialization/SerializationExamples.h"
#include "ConvAI/ConvAIExamples.h"
#include "ConvAI/ConvAITaskGraphExamples.h"

void AUnrealExamplesGameMode::StartPlay()
{
	Super::StartPlay();

	//SerializationExamples::AllExamples();

	//ConvAIExamples::AllExamples(GetWorld());

	ConvAITaskGraphExamples::AllExamples(GetWorld());
}


void AUnrealExamplesGameMode::Tick(float DeltaSeconds)
{
	SCOPE_CYCLE_COUNTER(STAT_Tick)

	Super::Tick(DeltaSeconds);

}