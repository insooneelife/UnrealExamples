// Fill out your copyright notice in the Description page of Project Settings.


#include "Dedicate/DedicateGameInstanceSubsystem.h"
#include "Engine/World.h"



void UDedicateGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UWorld* World = GetWorld();

	if (World != nullptr)
	{
		if (World->GetNetMode() == NM_Client)
		{
			UE_LOG(LogTemp, Log, TEXT("UDedicateGameInstanceSubsystem::Initialize  client"));
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("UDedicateGameInstanceSubsystem::Initialize  server"));
		}
	}
}

void UDedicateGameInstanceSubsystem::Deinitialize()
{


	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		if (World->GetNetMode() == NM_Client)
		{
			UE_LOG(LogTemp, Log, TEXT("UDedicateGameInstanceSubsystem::Deinitialize  client"));
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("UDedicateGameInstanceSubsystem::Deinitialize  server"));
		}
	}

	Super::Deinitialize();
}