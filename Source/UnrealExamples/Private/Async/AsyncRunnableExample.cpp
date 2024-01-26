#include "Async/AsyncRunnableExample.h"
#include "HAL/ThreadManager.h"



FTestRunnable::FTestRunnable()
	:
	Thread(nullptr), bStop(false)
{
	Thread = FRunnableThread::Create(this, TEXT("ThreadName"));
	Promise = MakeShared<TPromise<FString>>();
}

FTestRunnable::~FTestRunnable()
{
	if (Thread)
	{
		Thread->Kill();
		delete Thread;
	}
}

void FTestRunnable::Exit()
{
	UE_LOG(LogTemp, Display, TEXT("Thread Exit."));
}

bool FTestRunnable::Init()
{
	bStop = false;
	return true;
}

uint32 FTestRunnable::Run()
{
	int Count = 0;

	while (!bStop)
	{
		UE_LOG(LogTemp, Display, TEXT("Thread Running.."));
		FPlatformProcess::Sleep(1.0f);

		if (Count++ == 3)
		{
			break;
		}
	}

	Promise->SetValue(TEXT("Data is ready."));

	UE_LOG(LogTemp, Display, TEXT("Thread Run Finished."));
	return 0;
}

void FTestRunnable::Stop()
{
	bStop = true;
}

void AsyncRunnableExample::Start()
{	
	TestRunnable = MakeShared<FTestRunnable>();
	Future = MakeShared<TFuture<FString>>(TestRunnable->GetPromise()->GetFuture());
}

void AsyncRunnableExample::End()
{
}


void AsyncRunnableExample::Tick()
{
	if (Future.IsValid() && Future->IsReady())
	{
		UE_LOG(LogTemp, Error, TEXT("%s"), *Future->Get());
		Future.Reset();
	}
}