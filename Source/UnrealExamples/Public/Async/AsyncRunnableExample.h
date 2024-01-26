#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HAL/Runnable.h"


class UNREALEXAMPLES_API FTestRunnable : FRunnable
{
public:
	FTestRunnable();
	virtual ~FTestRunnable();

public:

	virtual void Exit() override;
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;

	TSharedPtr<TPromise<FString>> GetPromise() const { return Promise; }

private:
	TSharedPtr<TPromise<FString>> Promise;
	FRunnableThread* Thread;
	bool bStop;
};



class UNREALEXAMPLES_API AsyncRunnableExample
{
public:

	void Start();

	void Tick();

	void End();

private:
	TSharedPtr<FTestRunnable> TestRunnable;

	TSharedPtr<TFuture<FString>> Future;
};


