// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetworkMessage.h"
#include "TCPSocketClientUtils.h"

struct FRecvResult
{
public:
	bool bResult;
	TArray<uint8> Payload;
};

class UNREALEXAMPLES_API FTCPSocketClient_AsyncFuture
{
public:
	static TSharedPtr<FBufferArchive> CreatePacket(uint32 InType, const uint8* InPayload, int32 InPayloadSize);

	void Connect();
	void Disconnect();

	TSharedPtr<FBufferArchive> CreatePacket(uint32 Type, const FString& Text);

	void Tick();

private:

	TFuture<FRecvResult> SendAndRecv();

	TFuture<bool> BeginSend(TSharedPtr<FBufferArchive> InPacket);

	void BeginRecv(TSharedPtr<TPromise<FRecvResult>> InPromise);

	void HandleRecv(const FRecvResult& InResult);

private:
	FSocket* Socket;

	TSharedPtr<TFuture<FRecvResult>> RecvResult;
};