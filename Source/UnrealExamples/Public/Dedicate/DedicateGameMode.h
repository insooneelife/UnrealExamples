#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include "DedicateGameMode.generated.h"




UCLASS()
class UNREALEXAMPLES_API ADedicateGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	ADedicateGameMode();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);

	virtual void InitGameState();

	virtual void StartPlay();


	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaSeconds) override;


	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage);

	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage);

	virtual void PostLogin(APlayerController* NewPlayer);

	virtual void Logout(AController* Exiting);

	/** Called when the state transitions to WaitingToStart */
	virtual void HandleMatchIsWaitingToStart();

	/** Called when the state transitions to InProgress */
	virtual void HandleMatchHasStarted();

	/** Called when the map transitions to WaitingPostMatch */
	virtual void HandleMatchHasEnded();

	/** Called when the match transitions to LeavingMap */
	virtual void HandleLeavingMap();

	/** Called when the match transitions to Aborted */
	virtual void HandleMatchAborted();

	/** Handles when a player is disconnected, before the session does */
	virtual void HandleDisconnect(UWorld* InWorld, UNetDriver* NetDriver);

private:

};



