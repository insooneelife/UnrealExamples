// Copyright Epic Games, Inc. All Rights Reserved.


#include "UnrealExamplesGameMode.h"
#include "Serialization/SerializationExamples.h"

void AUnrealExamplesGameMode::StartPlay()
{
	Super::StartPlay();

	SerializationExamples::AllExamples();
}