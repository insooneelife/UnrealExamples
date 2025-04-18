#include "Async/AsyncTaskExample.h"
#include "HAL/ThreadManager.h"
#include "Async/Async.h"

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


