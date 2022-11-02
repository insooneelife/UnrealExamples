// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JsonSerializationExamples.generated.h"

USTRUCT(BlueprintType)
struct UNREALEXAMPLES_API FMyStruct
{
	GENERATED_USTRUCT_BODY()
public:
	FMyStruct()
	{}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> Strings;
};


class UNREALEXAMPLES_API JsonSerializationExamples
{
public:

	static void Array2DToJson();


	static void StructToJSON(const FMyStruct& InData, FString& OutJsonString);

	static void StructFromJSON(const FString& InJsonString, FMyStruct& OutData);
};
