// Fill out your copyright notice in the Description page of Project Settings.


#include "Networking/TCPSocketClient/TCPSocketClient_AsyncFuture.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "Serialization/ArrayWriter.h"
#include "SocketSubsystem.h"

#include "Networking.h"
#include "SocketSubsystemModule.h"


TSharedPtr<FBufferArchive> FTCPSocketClient_AsyncFuture::CreatePacket(
	uint32 InType, const uint8* InPayload, int32 InPayloadSize)
{
	// make a header for the payload
	FMessageHeader Header(InType, InPayloadSize);
	constexpr static int32 HeaderSize = sizeof(FMessageHeader);

	TSharedPtr<FBufferArchive> Packet = MakeShareable(new FBufferArchive());

	// serialize out the header
	(*Packet) << Header;

	// append the payload bytes to send it in one network packet
	Packet->Append(InPayload, InPayloadSize);

	return Packet;
}

void FTCPSocketClient_AsyncFuture::Connect()
{
	Socket = FTcpSocketBuilder(TEXT("ClientSocket"));

	FString IPAddress = TEXT("127.0.0.1");
	uint16 PortNumber = 11000;

	if (FTCPSocketClientUtils::Connect(Socket, IPAddress, PortNumber))
	{
		UE_LOG(LogTemp, Log, TEXT("Socket Connected"));
		Socket->SetNonBlocking(false);

		RecvResult = MakeShared<TFuture<FRecvResult>>(SendAndRecv());
	}
	else
	{
		FTCPSocketClientUtils::DestroySocket(Socket);
	}
}

void FTCPSocketClient_AsyncFuture::Disconnect()
{
	FTCPSocketClientUtils::DestroySocket(Socket);
	Socket = nullptr;
}


TSharedPtr<FBufferArchive> FTCPSocketClient_AsyncFuture::CreatePacket(uint32 Type, const FString& Text)
{
	SCOPE_CYCLE_COUNTER(STAT_Send);

	FTCHARToUTF8 Convert(*Text);
	FArrayWriter WriterArray;

	WriterArray.Serialize((UTF8CHAR*)Convert.Get(), Convert.Length());
	TSharedPtr<FBufferArchive> Packet = CreatePacket(Type, WriterArray.GetData(), WriterArray.Num());
	return Packet;
}

void FTCPSocketClient_AsyncFuture::Tick()
{
	if(RecvResult.IsValid())
	{
		if(RecvResult->IsReady())
		{
			// handle
			HandleRecv(RecvResult->Get());
			RecvResult.Reset();
		}
		else
		{
			// wait
		}
	}
	else
	{
		RecvResult = MakeShared<TFuture<FRecvResult>>(SendAndRecv());
	}
}

TFuture<FRecvResult> FTCPSocketClient_AsyncFuture::SendAndRecv()
{
	TSharedPtr<FBufferArchive> Packet = CreatePacket(0, TEXT("start packet"));
	TFuture<bool> SendFuture = BeginSend(Packet);

	TSharedPtr<TPromise<FRecvResult>> RecvPromise = MakeShared<TPromise<FRecvResult>>();

	SendFuture.Next([this, RecvPromise](bool Result)
		{
			BeginRecv(RecvPromise);
		});

	return RecvPromise->GetFuture();
}

TFuture<bool> FTCPSocketClient_AsyncFuture::BeginSend(TSharedPtr<FBufferArchive> InPacket)
{
	TSharedPtr<TPromise<bool>> Promise = MakeShared<TPromise<bool>>();

	AsyncTask(ENamedThreads::AnyThread, [this, Promise, InPacket]()
	{
		int32 NumSend;
		bool bSuccess = Socket->Send(InPacket->GetData(), InPacket->Num(), NumSend);

		Promise->SetValue(bSuccess);
	});

	return Promise->GetFuture();
}

void FTCPSocketClient_AsyncFuture::BeginRecv(TSharedPtr<TPromise<FRecvResult>> InPromise)
{
	AsyncTask(ENamedThreads::AnyThread, [this, InPromise]()
		{
			if (Socket == nullptr || this == nullptr)
			{
				return;
			}

			TArray<uint8> HeaderBuffer;
			int32 HeaderSize = sizeof(FMessageHeader);
			HeaderBuffer.AddZeroed(HeaderSize);

			// recv header
			bool bSuccessRecvHeader = false;
			int32 NumRead = 0;
			bSuccessRecvHeader = Socket->Recv(
				HeaderBuffer.GetData(), HeaderBuffer.Num(), NumRead, ESocketReceiveFlags::Type::WaitAll);

			FRecvResult Result;
			Result.bResult = false;

			if (bSuccessRecvHeader)
			{
				// recv payload					
				FMessageHeader Header;
				FMemoryReader Reader(HeaderBuffer);
				Reader << Header;

				int32 PayloadSize = Header.Size;				
				TArray<uint8>& Payload = Result.Payload;

				Payload.SetNumZeroed(PayloadSize);

				Result.bResult = Socket->Recv(
					Payload.GetData(), Payload.Num(), NumRead, ESocketReceiveFlags::Type::WaitAll);

				InPromise->SetValue(Result);
			}
			else
			{
				InPromise->SetValue(Result);
			}
		});
}


void FTCPSocketClient_AsyncFuture::HandleRecv(const FRecvResult& InResult)
{
	if(InResult.bResult)
	{
		FString Data(InResult.Payload.Num(), (char*)InResult.Payload.GetData());
		UE_LOG(LogTemp, Log, TEXT("OnRecvCompleted  recv data success.  data : %s  Payload : %d  size : %d"),
			*Data, InResult.Payload.Num(), Data.Len());
	}
}

