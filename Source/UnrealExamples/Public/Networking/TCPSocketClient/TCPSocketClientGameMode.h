// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TCPSocketClient_NonBlocking.h"
#include "TCPSocketClientGameMode.generated.h"


UCLASS()
class UNREALEXAMPLES_API ATCPSocketClientGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	virtual void StartPlay() override;

	virtual void Tick(float DeltaSeconds) override;

private:

	FTCPSocketClient_NonBlocking TCPClient_NonBlocking;
};

