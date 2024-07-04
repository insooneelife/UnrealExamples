// Fill out your copyright notice in the Description page of Project Settings.


#include "Dedicate/DedicateGameState.h"

void ADedicateGameState::BeginPlay()
{
	Super::BeginPlay();

	if (GetNetMode() == NM_Client)
	{
		UE_LOG(LogTemp, Error, TEXT("ADedicateGameState::BeginPlay  client"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ADedicateGameState::BeginPlay  server"));
	}
}

void ADedicateGameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (GetNetMode() == NM_Client)
	{
		UE_LOG(LogTemp, Error, TEXT("ADedicateGameState::EndPlay  client"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ADedicateGameState::EndPlay  server"));
	}
}