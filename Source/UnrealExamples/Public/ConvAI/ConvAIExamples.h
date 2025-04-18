// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

class UWorld;

class ConvAIExamples
{
public:

	static void AllExamples(UWorld* World);

	static void WhisperExample(UWorld* World);
	static void TTSExample(UWorld* World);
	static void ChatGPTExample(UWorld* World);
};
