// Fill out your copyright notice in the Description page of Project Settings.


#include "Networking/TCPSocketClient/TCPSocketClient_NonBlocking.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "Serialization/ArrayWriter.h"
#include "SocketSubsystem.h"

bool FTCPSocketClient_NonBlocking::Connect(FSocket* Socket, FString IPAddress, int PortNumber)
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
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Socket Connect Failed."));
		return false;
	}

}


// if data size is too big for just one recv, it needs to be called multi times.
bool FTCPSocketClient_NonBlocking::Receive(FSocket* Socket, uint8* Results, int32 Size)
{
	int32 Offset = 0;
	while (Size > 0)
	{
		int32 NumRead = 0;
		Socket->Recv(Results + Offset, Size, NumRead);
		check(NumRead <= Size);
		// make sure we were able to read at least something (and not too much)
		if (NumRead <= 0)
		{
			return false;
		}

		// if we read a partial block, move along
		Offset += NumRead;
		Size -= NumRead;
	}
	return true;
}

bool FTCPSocketClient_NonBlocking::Send(FSocket* Socket, const uint8* Buffer, int32 Size)
{
	while (Size > 0)
	{
		int32 BytesSent = 0;
		if (!Socket->Send(Buffer, Size, BytesSent))
		{
			return false;
		}

		Size -= BytesSent;
		Buffer += BytesSent;
	}

	return true;
}


bool FTCPSocketClient_NonBlocking::SendPacket(FSocket* Socket, uint32 Type, const TArray<uint8>& Payload)
{
	return SendPacket(Socket, Type, Payload.GetData(), Payload.Num());
}

bool FTCPSocketClient_NonBlocking::SendPacket(
	FSocket* Socket, uint32 Type, const uint8* Payload, int32 PayloadSize)
{
	// make a header for the payload
	FMessageHeader Header(Type, PayloadSize);
	constexpr static int32 HeaderSize = sizeof(FMessageHeader);

	// serialize out the header
	FBufferArchive Ar;
	Ar << Header;

	// append the payload bytes to send it in one network packet
	Ar.Append(Payload, PayloadSize);

	// Send it, and make sure it sent it all
	if (!Send(Socket, Ar.GetData(), Ar.Num()))
	{
		UE_LOG(LogSockets, Error, TEXT("Unable To Send."));
		return false;
	}
	return true;
}

bool FTCPSocketClient_NonBlocking::ReceivePacket(FSocket* Socket, TArray<uint8>& OutPayload)
{
	const int32 MaxPacketSize = 1024 * 1024;
	uint32 PendingDataSize;
	if (!Socket->HasPendingData(PendingDataSize) || PendingDataSize <= MaxPacketSize)
	{
		return false;
	}

	TArray<uint8> HeaderBuffer;
	int32 HeaderSize = sizeof(FMessageHeader);
	HeaderBuffer.AddZeroed(HeaderSize);

	// recv header
	int32 BytesRead = 0;
	if (!Receive(Socket, HeaderBuffer.GetData(), HeaderBuffer.Num()))
	{
		UE_LOG(LogTemp, Error, TEXT("Recv Header Failed."));
		return false;
	}

	FMessageHeader Header;
	{
		FMemoryReader Reader(HeaderBuffer);
		Reader << Header;
		UE_LOG(LogTemp, Log, TEXT("Recv Header Type : %d  Size : %d"), Header.Type, Header.Size);
	}

	int32 PayloadSize = Header.Size;
	OutPayload.SetNumZeroed(PayloadSize);


	if (Receive(Socket, OutPayload.GetData(), OutPayload.Num()))
	{
		return true;
	}

	return false;
}

void FTCPSocketClient_NonBlocking::PrintSocketError(const FString& Text)
{
	ESocketErrors Err = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLastErrorCode();
	const TCHAR* SocketErr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetSocketError(Err);
	UE_LOG(LogSockets, Error, TEXT("%s  SocketErr : %s"), *Text, SocketErr);
}

void FTCPSocketClient_NonBlocking::Connect()
{
	int32 RecvBufferSize = 2 * 1024 * 1024;

	Socket = FTcpSocketBuilder(TEXT("ClientSocket"));
	Socket->SetNonBlocking(true);
	
	Socket->SetReceiveBufferSize(RecvBufferSize, RecvBufferSize);

	FString IPAddress = TEXT("127.0.0.1");
	uint16 PortNumber = 11000;

	if (Connect(Socket, IPAddress, PortNumber))
	{
		UE_LOG(LogTemp, Log, TEXT("Socket Connected"));
	}
	else
	{
		DestroySocket();
	}
}

void FTCPSocketClient_NonBlocking::DestroySocket()
{
	if (Socket)
	{
		if (Socket->GetConnectionState() == SCS_Connected)
		{
			Socket->Close();
		}

		ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get();
		SocketSubsystem->DestroySocket(Socket);

		Socket = nullptr;
	}
}


void FTCPSocketClient_NonBlocking::Send(uint32 Type, const FString& Text)
{
	SCOPE_CYCLE_COUNTER(STAT_Send)
		FTCHARToUTF8 Convert(*Text);
	FArrayWriter WriterArray;
	WriterArray.Serialize((UTF8CHAR*)Convert.Get(), Convert.Length());

	if (FTCPSocketClient_NonBlocking::SendPacket(Socket, Type, WriterArray))
	{
		UE_LOG(LogTemp, Log, TEXT("Sent Text : %s  Size : %d"), *Text, WriterArray.Num());
	}
}

void FTCPSocketClient_NonBlocking::Recv()
{
	SCOPE_CYCLE_COUNTER(STAT_Recv)
		TArray<uint8> Payload;
	if (FTCPSocketClient_NonBlocking::ReceivePacket(Socket, Payload))
	{
		FString Data(Payload.Num(), (char*)Payload.GetData());
		UE_LOG(LogTemp, Log, TEXT("Recv data success.  data : %s  Payload : %d  size : %d"), *Data, Payload.Num(), Data.Len());
	}
}