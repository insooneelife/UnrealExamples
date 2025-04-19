// Fill out your copyright notice in the Description page of Project Settings.


#if WITH_LOW_LEVEL_TESTS

#include "ConvAI/ConvAITaskGraphExamples.h"

#include "ConvAIModule.h"


#include "TtsUtils.h"

#include "Http.h"
#include "Misc/Paths.h"

#include "Async/Async.h"
#include "Kismet/GameplayStatics.h"
#include "Audio.h"

#include "HAL/PlatformProcess.h"
#include "Async/TaskGraphInterfaces.h"



IMPLEMENT_SIMPLE_AUTOMATION_TEST(FConvAITaskGraphExamplesImplicitTest, "ConvAI.Implicit.FlowExecution", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FConvAITaskGraphExamplesImplicitTest::RunTest(const FString& Parameters)
{
	// true 를 반환하면 테스트 통과, false 를 반환하면 테스트 실패입니다.
	return true;
}



#endif 