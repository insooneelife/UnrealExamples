#include "Async/AsyncExamples.h"
#include "HAL/ThreadManager.h"


void FAsyncTaskExample::DoWork()
{
	FString FuncName(__FUNCTION__);
	uint32 ThreadId = FPlatformTLS::GetCurrentThreadId();

	UE_LOG(LogTemp, Warning, TEXT("%s  ThreadId : %d  Start"),
		*FuncName, ThreadId);

	// Simulating a time-consuming operation
	for (int32 I = 1; I <= 5; ++I)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s  ThreadId : %d  Count : %d"),
			*FuncName, ThreadId, I);

		// Simulate a delay (in seconds)
		FPlatformProcess::Sleep(1.0f);
	}

	UE_LOG(LogTemp, Warning, TEXT("%s  ThreadId : %d  End"),
		*FuncName, ThreadId);
}

void FAsyncTaskManager::AddTask(TSharedPtr<FAsyncTask<FAsyncTaskExample>> NewTask)
{
	if (NewTask)
	{
		Tasks.Add(NewTask);
	}
}

void FAsyncTaskManager::CancelAllTasks()
{
	for (TSharedPtr<FAsyncTask<FAsyncTaskExample>> Task : Tasks)
	{
		Task->EnsureCompletion();
	}

	Tasks.Empty();
}



void AsyncCustomTaskExample::Start()
{
	const int32 TaskNum = 10;

	for (int32 I = 0; I < TaskNum; ++I)
	{
		TSharedPtr<FAsyncTask<FAsyncTaskExample>> AsyncTask = MakeShared<FAsyncTask<FAsyncTaskExample>>();
		FAsyncTaskExample& Task = AsyncTask->GetTask();

		// do something with Task
		TaskManager.AddTask(AsyncTask);

		// The task is executed on the background thread pool
		// this function calls FAsyncTaskExample's Task method
		AsyncTask->StartBackgroundTask();
	}
}


void AsyncCustomTaskExample::End()
{
	// this waits all tasks be done.
	TaskManager.CancelAllTasks();
}



void AsyncTaskExample::Start()
{
	Future = CallAsyncJobs();
}

void AsyncTaskExample::Tick()
{
	if (Future.IsValid() && Future->IsReady())
	{
		FString Value = Future->Get();

		FString FuncName(__FUNCTION__);
		uint32 ThreadId = FPlatformTLS::GetCurrentThreadId();
		UE_LOG(LogTemp, Error, TEXT("%s  Tick  Value : %s  Thread : %d"), *FuncName, *Value, ThreadId);

		Future.Reset();
	}
}


TSharedPtr<TFuture<FString>> AsyncTaskExample::CallAsyncJobs()
{
	TSharedPtr<TPromise<FString>> Promise = MakeShared<TPromise<FString>>();

	AsyncTask(ENamedThreads::AnyThread, [Promise]()
		{
			FString FuncName(__FUNCTION__);
			uint32 ThreadId = FPlatformTLS::GetCurrentThreadId();

			UE_LOG(LogTemp, Error, TEXT("%s  Async Thread : %d"),
				*FuncName, ThreadId);

			Promise->SetValue(TEXT("blabla"));
		});

	return MakeShared<TFuture<FString>>(Promise->GetFuture());
}