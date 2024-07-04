// Fill out your copyright notice in the Description page of Project Settings.


#include "Dedicate/DedicatePlayerState.h"

void ADedicatePlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (GetNetMode() == NM_Client)
	{
		UE_LOG(LogTemp, Error, TEXT("ADedicatePlayerState::BeginPlay  client"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ADedicatePlayerState::BeginPlay  server"));
	}
}

void ADedicatePlayerState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (GetNetMode() == NM_Client)
	{
		UE_LOG(LogTemp, Error, TEXT("ADedicatePlayerState::EndPlay  client"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ADedicatePlayerState::EndPlay  server"));
	}
}