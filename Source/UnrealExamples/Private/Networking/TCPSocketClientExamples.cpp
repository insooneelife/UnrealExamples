// Fill out your copyright notice in the Description page of Project Settings.


#include "Networking/TCPSocketClientExamples.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "Serialization/ArrayWriter.h"
#include "SocketSubsystem.h"


bool FMessageHeader::WrapAndSendPayload(uint32 Type, const TArray<uint8>& Payload, const FSimpleAbstractSocket& Socket)
{
	return FMessageHeader::WrapAndSendPayload(Type, Payload.GetData(), Payload.Num(), Socket);
}

bool FMessageHeader::WrapAndSendPayload(
	uint32 Type, const uint8* Payload, int32 PayloadSize, const FSimpleAbstractSocket& Socket)
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
	if (!Socket.Send(Ar.GetData(), Ar.Num()))
	{
		UE_LOG(LogSockets, Error, TEXT("Unable to send."));
		return false;
	}
	return true;
}

bool FMessageHeader::ReceivePayload(TArray<uint8>& OutPayload, const FSimpleAbstractSocket& Socket)
{
	TArray<uint8> HeaderBuffer;
	int32 HeaderSize = sizeof(FMessageHeader);
	HeaderBuffer.AddZeroed(HeaderSize);

	// recv header
	int32 BytesRead = 0;
	if (!Socket.Receive(HeaderBuffer.GetData(), HeaderBuffer.Num()))
	{
		UE_LOG(LogTemp, Error, TEXT("Recv Header Failed."));
		return false;
	}

	// parse it as a header (doing any byte swapping as needed)
	FMessageHeader Header;
	{
		FMemoryReader Reader(HeaderBuffer);
		Reader << Header;
		UE_LOG(LogTemp, Log, TEXT("Recv Header Type : %d  Size : %d"), Header.Type, Header.Size);
	}

	int32 PayloadSize = Header.Size;
	OutPayload.SetNumZeroed(PayloadSize);


	if (Socket.Receive(OutPayload.GetData(), OutPayload.Num()))
	{
		return true;
	}

	return false;
}

void TCPSocketClientExamples::Init()
{
	Socket = FTcpSocketBuilder(TEXT("ClientSocket"));
	Socket->SetNonBlocking(true);
}


bool TCPSocketClientExamples::Connect()
{
	FString IPAddress = TEXT("127.0.0.1");
	uint16 PortNumber = 11000;

	FIPv4Address IPAddr;
	if (!FIPv4Address::Parse(IPAddress, IPAddr))
	{
		UE_LOG(LogTemp, Error, TEXT("Ill-formed IP Address"));
		return false;
	}

	FIPv4Endpoint Endpoint = FIPv4Endpoint(IPAddr, PortNumber);

	if (Socket->Connect(*Endpoint.ToInternetAddr()))
	{
		UE_LOG(LogTemp, Log, TEXT("Socket Connected"));
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Socket Connect Failed."));
		DestroySocket();
		return false;
	}

}

void TCPSocketClientExamples::Send(uint32 Type, const FString& Text)
{
	FTCHARToUTF8 Convert(*Text);
	FArrayWriter WriterArray;
	WriterArray.Serialize((UTF8CHAR*)Convert.Get(), Convert.Length());

	FSimpleAbstractSocket_FSocket SimpleSocket(Socket);

	if (FMessageHeader::WrapAndSendPayload(Type, WriterArray, SimpleSocket))
	{
		UE_LOG(LogTemp, Log, TEXT("Sent Text : %s  Size : %d"), *Text, WriterArray.Num());
	}
}

void TCPSocketClientExamples::Recv()
{	
	FSimpleAbstractSocket_FSocket SimpleSocket(Socket);
	TArray<uint8> Payload;
	FMessageHeader::ReceivePayload(Payload, SimpleSocket);

	FUTF8ToTCHAR Convert((char*)Payload.GetData());
	FString Data(Convert.Length(), (TCHAR*)Convert.Get());
	UE_LOG(LogTemp, Error, TEXT("Recv data success.  data : %s"), *Data);
}

void TCPSocketClientExamples::Poll()
{
	if (!Socket)
	{
		return;
	}

	Recv();
}

void TCPSocketClientExamples::DestroySocket()
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