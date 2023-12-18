#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Animation/Skeleton.h"
#include "AsyncExamples.generated.h"




class FAsyncTaskExample : public FNonAbandonableTask
{
	int32 MaxCount;
public:

	void DoWork();

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FAsyncTaskExample, STATGROUP_ThreadPoolAsyncTasks);
	}
};

class FAsyncTaskManager
{
	TArray<TSharedPtr<FAsyncTask<FAsyncTaskExample>>> Tasks;

public:
	void AddTask(TSharedPtr<FAsyncTask<FAsyncTaskExample>> NewTask);

	void CancelAllTasks();
};



class UNREALEXAMPLES_API AsyncCustomTaskExample
{
public:

	void Start();

	void End();

private:

	FAsyncTaskManager TaskManager;
};


class UNREALEXAMPLES_API AsyncTaskExample
{
public:

	void Start();
	void Tick();

private:
	TSharedPtr<TFuture<FString>> CallAsyncJobs();

private:
	TSharedPtr<TFuture<FString>> Future;
};





UCLASS()
class UNREALEXAMPLES_API UAsyncExampleObject : public UObject
{
	GENERATED_BODY()

public:

};