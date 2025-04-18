// Fill out your copyright notice in the Description page of Project Settings.


#include "Dedicate/DedicateWorldSubsystem.h"
#include "Engine/World.h"

void UDedicateWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UWorld* World = GetWorld();

	if(World != nullptr)
	{
		if (World->GetNetMode() == NM_Client)
		{
			UE_LOG(LogTemp, Error, TEXT("UDedicateWorldSubsystem::Initialize  client"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UDedicateWorldSubsystem::Initialize  server"));
		}
	}

}

void UDedicateWorldSubsystem::Deinitialize()
{

	UWorld* World = GetWorld();

	if (World != nullptr)
	{
		if (World->GetNetMode() == NM_Client)
		{
			UE_LOG(LogTemp, Error, TEXT("UDedicateWorldSubsystem::Deinitialize  client"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UDedicateWorldSubsystem::Deinitialize  server"));
		}
	}

	Super::Deinitialize();
}