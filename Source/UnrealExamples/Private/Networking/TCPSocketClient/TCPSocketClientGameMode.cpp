#include "Networking/TCPSocketClient/TCPSocketClientGameMode.h"


void ATCPSocketClientGameMode::StartPlay()
{
	Super::StartPlay();

	//TCPClient_NonBlocking.Connect();
	//TCPClient_Blocking.Connect();
	TCPSocketClient_Async.Connect();
}

void ATCPSocketClientGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//TCPClient_NonBlocking.Disconnect();
	//TCPClient_Blocking.Disconnect();

	TCPSocketClient_Async.Disconnect();

	Super::EndPlay(EndPlayReason);
}


void ATCPSocketClientGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//TCPClient_NonBlocking.Send(0, TEXT("This is sent from client."));
	//TCPClient_NonBlocking.Recv();

	//TCPClient_Blocking.Send(0, TEXT("This is sent from client."));
	//TCPClient_Blocking.Recv();
}