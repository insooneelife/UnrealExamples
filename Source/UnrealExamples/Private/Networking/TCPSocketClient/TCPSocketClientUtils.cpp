// Fill out your copyright notice in the Description page of Project Settings.

#include "Networking/TCPSocketClient/TCPSocketClientUtils.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "Serialization/ArrayWriter.h"
#include "SocketSubsystem.h"


bool FTCPSocketClientUtils::Connect(FSocket* Socket, FString IPAddress, int PortNumber)
{
	FIPv4Address IPAddr;
	if (!FIPv4Address::Parse(IPAddress, IPAddr))
	{
		UE_LOG(LogTemp, Error, TEXT("Ill-formed IP Address"));
		return false;
	}

	FIPv4Endpoint Endpoint = FIPv4Endpoint(IPAddr, PortNumber);

	if (Socket->Connect(*Endpoint.ToInternetAddr()))
	{
		if (Socket->GetConnectionState() == ESocketConnectionState::SCS_Connected)
		{
			return true;
		}
	}

	if (Socket->GetConnectionState() == ESocketConnectionState::SCS_Connected)
	{
		return true;
	}

	FString ErrorText;
	PrintSocketError(ErrorText);

	UE_LOG(LogTemp, Error, TEXT("Socket Connect Failed.  Error : %s  ConnectionState : %d"),
		*ErrorText, (int32)Socket->GetConnectionState());
	return false;

}


void FTCPSocketClientUtils::DestroySocket(FSocket* Socket)
{
	if (Socket)
	{
		if (Socket->GetConnectionState() == SCS_Connected)
		{
			Socket->Close();

			UE_LOG(LogTemp, Log, TEXT("Socket Closed."));
		}
	}
}


void FTCPSocketClientUtils::PrintSocketError(FString& OutText)
{
	ESocketErrors Err = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLastErrorCode();
	const TCHAR* SocketErr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetSocketError(Err);
	OutText = SocketErr;
}