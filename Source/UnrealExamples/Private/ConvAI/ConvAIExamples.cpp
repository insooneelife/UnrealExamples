// Fill out your copyright notice in the Description page of Project Settings.


#include "ConvAI/ConvAIExamples.h"

#include "ConvAIModule.h"
#include "TtsUtils.h"
#include "WhisperUtils.h"
#include "LLMAPITask.h"


#include "Http.h"
#include "Misc/Paths.h"

#include "Async/Async.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundWave.h"
#include "Sound/SoundWaveProcedural.h"
#include "Audio.h"
#include "Engine/World.h"
#include "HAL/PlatformProcess.h"
#include "Async/TaskGraphInterfaces.h"




void ConvAIExamples::AllExamples(UWorld* World)
{
	WhisperExample(World);
	TTSExample(World);
	ChatGPTExample(World);
}


void ConvAIExamples::WhisperExample(UWorld* World)
{

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
				UE_LOG(LogTemp, Log, TEXT("WhisperExample  %s"), *OutResponse.text);
			}
		});

	// Execute request
	Request->ProcessRequest();
	
}

void ConvAIExamples::TTSExample(UWorld* World)
{

	FTTSRequestBody Body;
	Body.Data = "hello, my name is gjklagjflkdgjdklf.";

	TSharedRef<IHttpRequest> Request = FTtsUtils::CreateRequest(Body);

	// Set response callback
	Request->OnProcessRequestComplete().BindLambda(
		[World](FHttpRequestPtr InRequest, FHttpResponsePtr InResponse, bool bWasSuccessful)
		{
			if (!bWasSuccessful || !InResponse.IsValid())
			{
				UE_LOG(LogTemp, Error, TEXT("TTS Response failed."));
				return;
			}

			if(!IsValid(World))
			{
				return;
			}


			const TArray<uint8>& AudioBuffer = InResponse->GetContent();
			UE_LOG(LogTemp, Log, TEXT("TTSExample  AudioBuffer : %d"), AudioBuffer.Num());

			USoundWaveProcedural* SoundWav = FConvAIModule::CreateSoundWaveFromWav(AudioBuffer);

			if (IsValid(SoundWav))
			{
				UE_LOG(LogTemp, Log, TEXT("TTSExample  Play sound."));
				UGameplayStatics::PlaySound2D(World, SoundWav);
			}
		});

	// Execute request
	Request->ProcessRequest();
	
}

void ConvAIExamples::ChatGPTExample(UWorld* World)
{
	FString Message = TEXT("tell me about what llm is.");
	FString APIKey = FChatGPTUtils::GetAPIKey();
	TSharedRef<IHttpRequest> Request = FChatGPTUtils::CreateRequest(APIKey, Message);

	Request->OnProcessRequestComplete().BindLambda(
		[](FHttpRequestPtr InRequest, FHttpResponsePtr InResponse, bool bWasSuccessful)
		{
			if (!bWasSuccessful || !InResponse.IsValid())
			{
				return;
			}

			FChatGPTResponse ResponseData;
			if (FChatGPTUtils::ParseResponse(InResponse, ResponseData))
			{
				if(ResponseData.choices.Num() > 0 )
				{
					UE_LOG(LogTemp, Log, TEXT("LLM response.  %s"), *ResponseData.choices[0].message.content);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Empty message.  check your api key."));
				}
			}

		});

	// Execute request
	Request->ProcessRequest();
}

