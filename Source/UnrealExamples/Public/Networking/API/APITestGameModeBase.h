#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Sound/SoundWave.h"
#include "APIDataTypes.h"
#include "APIHandler.h"
#include "APITestGameModeBase.generated.h"


UCLASS()
class UNREALEXAMPLES_API AAPITestGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	AAPITestGameModeBase(const class FObjectInitializer& ObjectInitializer);

	virtual void StartPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

	UFUNCTION(BlueprintCallable)
		void TestRequest(const FSampleRequestBody& InRequestBody);
	void OnResponse(const FSampleResponseBody& InResponseBody);


public:


private:
	TSharedPtr<FAPIHandler> APIHandler;
	FDelegateHandle ResponseHandle;
};