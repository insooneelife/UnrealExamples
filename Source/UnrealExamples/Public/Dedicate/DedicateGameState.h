// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "DedicateGameState.generated.h"

/**
 * 
 */
UCLASS()
class UNREALEXAMPLES_API ADedicateGameState : public AGameState
{
	GENERATED_BODY()
public:

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
