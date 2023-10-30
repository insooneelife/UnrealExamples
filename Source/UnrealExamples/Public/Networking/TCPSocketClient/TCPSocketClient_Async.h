// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetworkMessage.h"
#include "TCPSocketClientUtils.h"



class UNREALEXAMPLES_API FTCPSocketClient_Async
{
public:
	static TSharedPtr<FBufferArchive> CreatePacket(uint32 InType, const uint8* InPayload, int32 InPayloadSize);

	void Connect();
	void Disconnect();

	TSharedPtr<FBufferArchive> CreatePacket(uint32 Type, const FString& Text);

private:
	// all phase functions will be called at GameThread
	void BeginSendPhase();
	void EndSendPhase();

	void BeginRecvPhase();
	void EndRecvPhase();


	void OnSendCompleted();
	void OnSendFailed();

	void OnRecvCompleted(const TArray<uint8>& InPayload);
	void OnRecvFailed();


private:
	FSocket* Socket;
};