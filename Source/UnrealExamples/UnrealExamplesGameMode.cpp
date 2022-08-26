// Copyright Epic Games, Inc. All Rights Reserved.


#include "UnrealExamplesGameMode.h"
#include "Serialization/SerializationExamples.h"

void AUnrealExamplesGameMode::StartPlay()
{
	Super::StartPlay();

	SerializationExamples::AllExamples();

	

	Ex.Init();

	Ex.Connect();

	Ex.Send(0, TEXT("This is sent from client."));
	//Ex.DestroySocket();
}


void AUnrealExamplesGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	Ex.Poll();
}