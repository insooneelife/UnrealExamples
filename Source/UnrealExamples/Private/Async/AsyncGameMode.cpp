#include "Async/AsyncGameMode.h"
#include "HAL/ThreadManager.h"

#include "Async/AsyncExamples.h"

void AAsyncGameMode::BeginPlay()
{
	Super::BeginPlay();

	//AsyncCustomTaskExample.Start();

	//AsyncTaskExample.Start();


	AsyncRunnableExample.Start();
}

void AAsyncGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//AsyncCustomTaskExample.End();

	Super::EndPlay(EndPlayReason);
}



void AAsyncGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//AsyncTaskExample.Tick();

	AsyncRunnableExample.Tick();
}