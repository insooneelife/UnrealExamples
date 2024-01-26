#include "Async/AsyncCustomTaskExample.h"
#include "HAL/ThreadManager.h"


void FTestAsyncTask::DoWork()
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

void FAsyncTaskManager::AddTask(TSharedPtr<FAsyncTask<FTestAsyncTask>> NewTask)
{
	if (NewTask)
	{
		Tasks.Add(NewTask);
	}
}

void FAsyncTaskManager::CancelAllTasks()
{
	for (TSharedPtr<FAsyncTask<FTestAsyncTask>> Task : Tasks)
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
		TSharedPtr<FAsyncTask<FTestAsyncTask>> AsyncTask = MakeShared<FAsyncTask<FTestAsyncTask>>();
		FTestAsyncTask& Task = AsyncTask->GetTask();

		// do something with Task
		TaskManager.AddTask(AsyncTask);

		// The task is executed on the background thread pool
		// this function calls FTestAsyncTask's Task method
		AsyncTask->StartBackgroundTask();
	}
}


void AsyncCustomTaskExample::End()
{
	// this waits all tasks be done.
	TaskManager.CancelAllTasks();
}


