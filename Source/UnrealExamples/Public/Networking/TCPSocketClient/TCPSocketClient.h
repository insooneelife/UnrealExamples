// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetworkMessage.h"

DECLARE_STATS_GROUP(TEXT("TCPSocketClient"), STATGROUP_TCPSocketClient, STATCAT_Advanced);
DECLARE_CYCLE_STAT(TEXT("Send"), STAT_Send, STATGROUP_TCPSocketClient);
DECLARE_CYCLE_STAT(TEXT("Recv"), STAT_Recv, STATGROUP_TCPSocketClient);

class FSocket;

struct UNREALEXAMPLES_API FMessageHeader
{
	uint32 Type;
	uint32 Size;

	FMessageHeader()
		: Type(0), Size(0)
	{
	}

	FMessageHeader(uint32 Type, int32 PayloadSize)
		: Type(Type), Size(PayloadSize)
	{
	}

	friend FArchive& operator<<(FArchive& Ar, FMessageHeader& Header)
	{
		Ar << Header.Type;
		Ar << Header.Size;
		return Ar;
	}
};


class UNREALEXAMPLES_API TCPSocketClient
{
public:
	static bool Connect(FSocket* Socket, FString IPAddress, int PortNumber);

	static bool Receive(FSocket* Socket, uint8* Results, int32 Size);

	static bool Send(FSocket* Socket, const uint8* Buffer, int32 Size);

	static bool SendPacket(FSocket* Socket, uint32 Type, const TArray<uint8>& Payload);

	static bool SendPacket(FSocket* Socket, uint32 Type, const uint8* Payload, int32 PayloadSize);

	static bool ReceivePacket(FSocket* Socket, TArray<uint8>& OutPayload);

	void Connect();

	void DestroySocket();

	void Send(uint32 Type, const FString& Text);

	void Recv();

private:
	FSocket* Socket;
};