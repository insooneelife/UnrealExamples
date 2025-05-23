// Fill out your copyright notice in the Description page of Project Settings.


#include "Networking/TCPSocketClient/TCPSocketClient_Blocking.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "Serialization/ArrayWriter.h"
#include "Serialization/MemoryReader.h"
#include "SocketSubsystem.h"


// if data size is too big for just one recv, it needs to be called multi times.
bool FTCPSocketClient_Blocking::Receive(FSocket* Socket, uint8* Results, int32 Size)
{
	int32 NumRead = 0;
	bool bSuccess = Socket->Recv(Results, Size, NumRead, ESocketReceiveFlags::Type::WaitAll);

	return bSuccess;
}

bool FTCPSocketClient_Blocking::Send(FSocket* Socket, const uint8* Buffer, int32 Size)
{
	int32 BytesSent = 0;
	bool bSuccess = Socket->Send(Buffer, Size, BytesSent);
	return bSuccess;
}


bool FTCPSocketClient_Blocking::SendPacket(FSocket* Socket, uint32 Type, const TArray<uint8>& Payload)
{
	return SendPacket(Socket, Type, Payload.GetData(), Payload.Num());
}

bool FTCPSocketClient_Blocking::SendPacket(
	FSocket* Socket, uint32 Type, const uint8* Payload, int32 PayloadSize)
{
	// make a header for the payload
	FMessageHeader Header(Type, PayloadSize);
	constexpr static int32 HeaderSize = sizeof(FMessageHeader);

	// serialize out the header
	FArrayWriter Ar;
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

bool FTCPSocketClient_Blocking::ReceivePacket(FSocket* Socket, TArray<uint8>& OutPayload)
{
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

void FTCPSocketClient_Blocking::Connect()
{
	Socket = FTcpSocketBuilder(TEXT("ClientSocket"));

	FString IPAddress = TEXT("127.0.0.1");
	uint16 PortNumber = 11000;

	if (FTCPSocketClientUtils::Connect(Socket, IPAddress, PortNumber))
	{
		UE_LOG(LogTemp, Log, TEXT("Socket Connected"));
		Socket->SetNonBlocking(false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Socket Connect Failed."));

		FTCPSocketClientUtils::DestroySocket(Socket);
	}
}

void FTCPSocketClient_Blocking::Disconnect()
{
	FTCPSocketClientUtils::DestroySocket(Socket);
}

void FTCPSocketClient_Blocking::Send(uint32 Type, const FString& Text)
{
	SCOPE_CYCLE_COUNTER(STAT_Send);
	FTCHARToUTF8 Convert(*Text);
	FArrayWriter WriterArray;
	WriterArray.Serialize((UTF8CHAR*)Convert.Get(), Convert.Length());

	if (FTCPSocketClient_Blocking::SendPacket(Socket, Type, WriterArray))
	{
		UE_LOG(LogTemp, Log, TEXT("Sent Text : %s  Size : %d"), *Text, WriterArray.Num());
	}
}

void FTCPSocketClient_Blocking::Recv()
{
	SCOPE_CYCLE_COUNTER(STAT_Recv);

	TArray<uint8> Payload;

	if (FTCPSocketClient_Blocking::ReceivePacket(Socket, Payload))
	{
		FString Data(Payload.Num(), (char*)Payload.GetData());
		UE_LOG(LogTemp, Log, TEXT("Recv data success.  data : %s  Payload : %d  size : %d"), *Data, Payload.Num(), Data.Len());
	}
}