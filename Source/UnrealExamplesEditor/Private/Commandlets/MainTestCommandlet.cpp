#include "Commandlets/MainTestCommandlet.h"
//#include "ConvAI/ConvAITaskGraphExamples.h"
#include "Engine/World.h"
#include "Engine/Engine.h"


int32 UMainTestCommandlet::Main(const FString& Params)
{
    
	/*
	UE_LOG(LogTemp, Display, TEXT("TestCommandlet started with params: %s"), *Params);

    // Editor의 World 객체 획득
    UWorld* World = GEditor->GetEditorWorldContext().World();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get Editor World object"));
        return -1;
    }

    // ConvAITaskGraphExamples 테스트 실행
    ConvAITaskGraphExamples::AllExamples(World);
	*/
    UE_LOG(LogTemp, Display, TEXT("TestCommandlet completed successfully"));
    return 0;
} 