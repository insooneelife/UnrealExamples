// Fill out your copyright notice in the Description page of Project Settings.


#include "Serialization/JsonSerializationExamples.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "JsonObjectConverter.h"

void JsonSerializationExamples::Array2DToJson()
{
	TArray<TArray<FString>> Array2D =
	{
		{
			TEXT("aaa"), TEXT("aaa"), TEXT("aaa")
		},
		{
			TEXT("bbb"), TEXT("bbb"), TEXT("bbb")
		},
		{
			TEXT("ccc"), TEXT("ccc"), TEXT("ccc")
		},
	};

	FString OutJson;
	TArray<TSharedPtr<FJsonValue>> JsonValuesArray2D;

	for (int32 I = 0; I < Array2D.Num(); ++I)
	{
		TArray<TSharedPtr<FJsonValue>> JsonValuesArray;
		for (int32 J = 0; J < Array2D.Num(); ++J)
		{
			Array2D[I][J];

			TSharedPtr<FJsonValue> Elem = MakeShareable(new FJsonValueString(TEXT("aaa")));
			JsonValuesArray.Add(Elem);
		}

		TSharedPtr<FJsonValueArray> ArrayElem = MakeShareable(new FJsonValueArray(JsonValuesArray));
		JsonValuesArray2D.Add(ArrayElem);

		TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&OutJson);
		FJsonSerializer::Serialize(JsonValuesArray2D, JsonWriter);
	}

	UE_LOG(LogTemp, Error, TEXT("%s"), *OutJson);
}

void JsonSerializationExamples::StructToJSON(const FMyStruct& InData, FString& OutJsonString)
{
	FJsonObjectConverter::UStructToJsonObjectString(InData, OutJsonString);
}


void JsonSerializationExamples::StructFromJSON(const FString& InJsonString, FMyStruct& OutData)
{
	FJsonObjectConverter::JsonObjectStringToUStruct(InJsonString, &OutData);
}