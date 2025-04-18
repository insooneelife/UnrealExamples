// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stats/Stats.h"

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


DECLARE_STATS_GROUP(TEXT("TCPSocketClient"), STATGROUP_TCPSocketClient, STATCAT_Advanced);
DECLARE_CYCLE_STAT(TEXT("Send"), STAT_Send, STATGROUP_TCPSocketClient);
DECLARE_CYCLE_STAT(TEXT("Recv"), STAT_Recv, STATGROUP_TCPSocketClient);



class UNREALEXAMPLES_API FTCPSocketClientUtils
{
public:
	static bool Connect(FSocket* Socket, FString IPAddress, int PortNumber);

	static void DestroySocket(FSocket* Socket);

	static void PrintSocketError(FString& OutText);
};