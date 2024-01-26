#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include "Async/AsyncWork.h"
#include "AsyncExamples.h"

#include "AsyncGameMode.generated.h"


UCLASS()
class UNREALEXAMPLES_API AAsyncGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaSeconds) override;


private:


	AsyncTaskExample AsyncTaskExample;

	AsyncCustomTaskExample AsyncCustomTaskExample;

	AsyncRunnableExample AsyncRunnableExample;
};



