#include "Dedicate/DedicateGameMode.h"



ADedicateGameMode::ADedicateGameMode()
{
	if (GetNetMode() == NM_Client)
	{
		UE_LOG(LogTemp, Error, TEXT("ADedicateGameMode::ADedicateGameMode  client"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ADedicateGameMode::ADedicateGameMode  server"));
	}
}

void ADedicateGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void ADedicateGameMode::InitGameState()
{
	Super::InitGameState();
}

void ADedicateGameMode::StartPlay()
{
	Super::StartPlay();
}


void ADedicateGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (GetNetMode() == NM_Client)
	{
		UE_LOG(LogTemp, Error, TEXT("ADedicateGameMode::BeginPlay  client"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ADedicateGameMode::BeginPlay  server"));
	}
}

void ADedicateGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (GetNetMode() == NM_Client)
	{
		UE_LOG(LogTemp, Error, TEXT("ADedicateGameMode::EndPlay  client"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ADedicateGameMode::EndPlay  server"));
	}
}



void ADedicateGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}




void ADedicateGameMode::PreLogin(
	const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

APlayerController* ADedicateGameMode::Login(
	UPlayer* NewPlayer, 
	ENetRole InRemoteRole,
	const FString& Portal, 
	const FString& Options, 
	const FUniqueNetIdRepl& UniqueId, 
	FString& ErrorMessage)
{
	return Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
}

void ADedicateGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void ADedicateGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

void ADedicateGameMode::HandleMatchIsWaitingToStart()
{
	Super::HandleMatchIsWaitingToStart();
}

void ADedicateGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
}

void ADedicateGameMode::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
}

void ADedicateGameMode::HandleLeavingMap()
{
	Super::HandleLeavingMap();
}

void ADedicateGameMode::HandleMatchAborted()
{
	Super::HandleMatchAborted();
}

void ADedicateGameMode::HandleDisconnect(UWorld* InWorld, UNetDriver* NetDriver)
{
	Super::HandleDisconnect(InWorld, NetDriver);
}