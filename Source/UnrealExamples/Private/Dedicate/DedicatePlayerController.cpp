// Fill out your copyright notice in the Description page of Project Settings.


#include "Dedicate/DedicatePlayerController.h"



void ADedicatePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (GetNetMode() == NM_Client)
	{
		UE_LOG(LogTemp, Error, TEXT("ADedicatePlayerController::BeginPlay  client"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ADedicatePlayerController::BeginPlay  server"));
	}
}

void ADedicatePlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (GetNetMode() == NM_Client)
	{
		UE_LOG(LogTemp, Error, TEXT("ADedicatePlayerController::EndPlay  client"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ADedicatePlayerController::EndPlay  server"));
	}
}
