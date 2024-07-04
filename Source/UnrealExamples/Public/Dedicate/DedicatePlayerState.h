// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DedicatePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class UNREALEXAMPLES_API ADedicatePlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
