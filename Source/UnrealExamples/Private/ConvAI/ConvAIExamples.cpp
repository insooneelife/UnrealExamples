// Fill out your copyright notice in the Description page of Project Settings.


#include "ConvAI/ConvAIExamples.h"
//#include "STTAPI/Public/WhisperUtils.h"
//#include "ConvAI/Public/ConvAIModule.h"
//#include "TtsUtils.h"
#include "Http.h"


void ConvAIExamples::AllExamples()
{
	WhisperExample();
	TTSExample();
	ChatGPTByTaskExample();
}


void ConvAIExamples::WhisperExample()
{
/*
	FString RelativePath = FPaths::ProjectDir() / TEXT("Source/Python/output/audio.wav");
	TArray<uint8> Buffer;
	if (!FConvAIModule::LoadWavFileToBuffer(RelativePath, Buffer))
	{
		UE_LOG(LogTemp, Error, TEXT("Load wave file failed.  Path : %s"), *RelativePath);
		return;
	}

	TSharedRef<IHttpRequest> Request = FWhisperUtils::CreateRequest(Buffer);

	// Set response callback
	Request->OnProcessRequestComplete().BindLambda(
		[](FHttpRequestPtr InRequest, FHttpResponsePtr InResponse, bool bWasSuccessful)
		{
			if (!bWasSuccessful || !InResponse.IsValid())
			{
				return;
			}

			FWhisperResponse OutResponse;
			if (FWhisperUtils::ParseResponse(InResponse, OutResponse))
			{
				UE_LOG(LogTemp, Error, TEXT("%s"), *OutResponse.text);
			}
		});

	// Execute request
	Request->ProcessRequest();
	*/
}

void ConvAIExamples::TTSExample()
{
/*
	FTTSRequestBody Body;
	Body.Data = "hello, my name is gjklagjflkdgjdklf.";

	TSharedRef<IHttpRequest> Request = FTtsUtils::CreateRequest(Body);

	// Set response callback
	Request->OnProcessRequestComplete().BindLambda(
		[](FHttpRequestPtr InRequest, FHttpResponsePtr InResponse, bool bWasSuccessful)
		{
			if (!bWasSuccessful || !InResponse.IsValid())
			{
				return;
			}

			const TArray<uint8>& AudioBuffer = InResponse->GetContent();
			UE_LOG(LogTemp, Error, TEXT("AudioBuffer : %d"), AudioBuffer.Num());

		});

	// Execute request
	Request->ProcessRequest();
	*/
}

void ConvAIExamples::ChatGPTByTaskExample()
{

}