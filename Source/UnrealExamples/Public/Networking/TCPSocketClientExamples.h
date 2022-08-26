// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetworkMessage.h"

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

	static bool WrapAndSendPayload(uint32 Type, const TArray<uint8>& Payload, const FSimpleAbstractSocket& Socket);

	static bool WrapAndSendPayload(uint32 Type, const uint8* Payload, int32 PayloadSize, const FSimpleAbstractSocket& Socket);

	static bool ReceivePayload(TArray<uint8>& OutPayload, const FSimpleAbstractSocket& Socket);
};


class FSocket;
class UNREALEXAMPLES_API TCPSocketClientExamples
{
public:

	void Init();

	bool Connect();

	void Send(uint32 Type, const FString& Text);

	void Recv();

	void Poll();

	void DestroySocket();

private:

	FSocket* Socket;
};

