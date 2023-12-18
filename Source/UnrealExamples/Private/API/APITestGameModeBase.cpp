// Fill out your copyright notice in the Description page of Project Settings.


#include "API/APITestGameModeBase.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "JsonObjectConverter.h"

#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

#include "Misc/Base64.h"

AAPITestGameModeBase::AAPITestGameModeBase(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	APIHandler = MakeShareable(new FAPIHandler());
}

void AAPITestGameModeBase::StartPlay()
{
	Super::StartPlay();

	ResponseHandle = APIHandler->OnResponseSample.AddUObject(
		this, &AAPITestGameModeBase::OnResponse);

	FSampleRequestBody Body;

	Body.Data = TEXT("fhdskhbkf");

	TestRequest(Body);
}

void AAPITestGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	APIHandler->OnResponseSample.Remove(ResponseHandle);

	Super::EndPlay(EndPlayReason);
}

// tests
void AAPITestGameModeBase::TestRequest(const FSampleRequestBody& InRequestBody)
{
	APIHandler->RequestSample(InRequestBody);
}




void AAPITestGameModeBase::OnResponse(const FSampleResponseBody& InResponseBody)
{
	UE_LOG(LogTemp, Log, TEXT("AAPITestGameModeBase  OnResponse"));
}