// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetworkMessage.h"
#include "TCPSocketClientUtils.h"

class UNREALEXAMPLES_API FTCPSocketClient_Async
{
public:
	static bool Receive(FSocket* Socket, uint8* Results, int32 Size);

	static bool Send(FSocket* Socket, const uint8* Buffer, int32 Size);

	static bool SendPacket(FSocket* Socket, uint32 Type, const TArray<uint8>& Payload);

	static bool SendPacket(FSocket* Socket, uint32 Type, const uint8* Payload, int32 PayloadSize);

	static bool ReceivePacket(FSocket* Socket, TArray<uint8>& OutPayload);

	void Connect();

	void Disconnect();

	void Send(uint32 Type, const FString& Text);

	void Recv();

private:
	FSocket* Socket;
};