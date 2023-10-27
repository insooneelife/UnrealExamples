// Fill out your copyright notice in the Description page of Project Settings.


#include "Networking/TCPSocketClient/TCPSocketClientGameMode.h"


void ATCPSocketClientGameMode::StartPlay()
{
	Super::StartPlay();

	TCPClient_NonBlocking.Connect();
}


void ATCPSocketClientGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UE_LOG(LogTemp, Log, TEXT("Tick"));
	TCPClient_NonBlocking.Send(0, TEXT("This is sent from client."));
	TCPClient_NonBlocking.Recv();
}