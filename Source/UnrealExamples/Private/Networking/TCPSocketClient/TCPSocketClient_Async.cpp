// Fill out your copyright notice in the Description page of Project Settings.


#include "Networking/TCPSocketClient/TCPSocketClient_Async.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "Serialization/ArrayWriter.h"
#include "SocketSubsystem.h"

#include "Networking.h"
#include "SocketSubsystemModule.h"


TSharedPtr<FBufferArchive> FTCPSocketClient_Async::CreatePacket(
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

void FTCPSocketClient_Async::Connect()
{
	Socket = FTcpSocketBuilder(TEXT("ClientSocket"));

	FString IPAddress = TEXT("127.0.0.1");
	uint16 PortNumber = 11000;

	if (FTCPSocketClientUtils::Connect(Socket, IPAddress, PortNumber))
	{
		UE_LOG(LogTemp, Log, TEXT("Socket Connected"));
		Socket->SetNonBlocking(false);

		BeginSendPhase();
	}
	else
	{
		FTCPSocketClientUtils::DestroySocket(Socket);
	}
}

void FTCPSocketClient_Async::Disconnect()
{
	FTCPSocketClientUtils::DestroySocket(Socket);
	Socket = nullptr;
}


TSharedPtr<FBufferArchive> FTCPSocketClient_Async::CreatePacket(uint32 Type, const FString& Text)
{
	SCOPE_CYCLE_COUNTER(STAT_Send);

	FTCHARToUTF8 Convert(*Text);
	FArrayWriter WriterArray;

	WriterArray.Serialize((UTF8CHAR*)Convert.Get(), Convert.Length());
	TSharedPtr<FBufferArchive> Packet = CreatePacket(Type, WriterArray.GetData(), WriterArray.Num());
	return Packet;
}

void FTCPSocketClient_Async::BeginSendPhase()
{
	UE_LOG(LogTemp, Log, TEXT("BeginSendPhase"));
	TSharedPtr<FBufferArchive> Packet = CreatePacket(0, TEXT("start packet"));


	AsyncTask(ENamedThreads::AnyThread, [this, Packet]()
		{
			if (Socket == nullptr || this == nullptr)
			{
				return;
			}

			// send all things in queue
			int32 NumSend;
			bool bSuccess = Socket->Send(Packet->GetData(), Packet->Num(), NumSend);

			AsyncTask(ENamedThreads::GameThread, [this, bSuccess]()
				{
					if (Socket == nullptr || this == nullptr)
					{
						return;
					}

					if (bSuccess)
					{
						// send complete
						OnSendCompleted();
						EndSendPhase();
					}
					else
					{
						// send failed
						OnSendFailed();
						EndSendPhase();
					}
				});
		});
}

void FTCPSocketClient_Async::EndSendPhase()
{
	UE_LOG(LogTemp, Log, TEXT("EndSendPhase"));

	BeginRecvPhase();
}

void FTCPSocketClient_Async::BeginRecvPhase()
{
	UE_LOG(LogTemp, Log, TEXT("BeginRecvPhase"));

	AsyncTask(ENamedThreads::AnyThread, [this]()
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

			if (bSuccessRecvHeader)
			{
				// recv payload					
				FMessageHeader Header;
				FMemoryReader Reader(HeaderBuffer);
				Reader << Header;

				int32 PayloadSize = Header.Size;
				TArray<uint8> Payload;
				Payload.SetNumZeroed(PayloadSize);

				bool bSuccessRecvPayload = false;
				bSuccessRecvPayload = Socket->Recv(
					Payload.GetData(), Payload.Num(), NumRead, ESocketReceiveFlags::Type::WaitAll);

				AsyncTask(ENamedThreads::GameThread, [this, bSuccessRecvPayload, Payload]()
					{
						if (Socket == nullptr || this == nullptr)
						{
							return;
						}

						if (bSuccessRecvPayload)
						{
							OnRecvCompleted(Payload);
							EndRecvPhase();
						}
						else
						{
							UE_LOG(LogTemp, Error, TEXT("Recv Payload Failed."));
							OnRecvFailed();
							EndRecvPhase();
						}
					});
			}
			else
			{
				AsyncTask(ENamedThreads::GameThread, [this]()
					{
						if (Socket == nullptr || this == nullptr)
						{
							return;
						}

						UE_LOG(LogTemp, Error, TEXT("Recv Header Failed."));
						OnRecvFailed();
						EndRecvPhase();
					});
			}
		});
}

void FTCPSocketClient_Async::EndRecvPhase()
{
	UE_LOG(LogTemp, Log, TEXT("EndRecvPhase"));

	BeginSendPhase();
}

void FTCPSocketClient_Async::OnSendCompleted()
{
	UE_LOG(LogTemp, Log, TEXT("OnSendCompleted"));
}

void FTCPSocketClient_Async::OnSendFailed()
{
	FString OutText;
	FTCPSocketClientUtils::PrintSocketError(OutText);
	UE_LOG(LogTemp, Log, TEXT("OnSendFailed  Error : %s"), *OutText);
}

void FTCPSocketClient_Async::OnRecvCompleted(const TArray<uint8>& InPayload)
{
	TArray<uint8> Payload;
	Payload.Append(InPayload);

	FString Data(Payload.Num(), (char*)Payload.GetData());
	UE_LOG(LogTemp, Log, TEXT("OnRecvCompleted  recv data success.  data : %s  Payload : %d  size : %d"), *Data, Payload.Num(), Data.Len());
}

void FTCPSocketClient_Async::OnRecvFailed()
{
	FString OutText;
	FTCPSocketClientUtils::PrintSocketError(OutText);
	UE_LOG(LogTemp, Log, TEXT("OnRecvFailed  Error : %s"), *OutText);
}