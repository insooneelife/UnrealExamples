#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"


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


