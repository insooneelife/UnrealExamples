#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"


class FTestAsyncTask : public FNonAbandonableTask
{
	int32 MaxCount;
public:

	void DoWork();

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FTestAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}
};

class FAsyncTaskManager
{
	TArray<TSharedPtr<FAsyncTask<FTestAsyncTask>>> Tasks;

public:
	void AddTask(TSharedPtr<FAsyncTask<FTestAsyncTask>> NewTask);

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

