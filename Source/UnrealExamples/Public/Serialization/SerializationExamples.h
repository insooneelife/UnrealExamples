// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Animation/Skeleton.h"
#include "SerializationExamples.generated.h"

class UNREALEXAMPLES_API SerializationExamples
{
public:

	static void AllExamples();

	static void BaseExample();

	static void StructExample();

	static void ObjectExample();

	static void FileExample();

	static void MemoryAndFileExample();
};


struct SerializationExampleData
{
	uint32 A;
	uint32 B;
	FString C;

	// for read / write serialization for this struct
	friend FArchive& operator<<(FArchive& Ar, SerializationExampleData& Data)
	{
		Ar << Data.A;
		Ar << Data.B;
		Ar << Data.C;
		return Ar;
	}
};


UENUM(BlueprintType)
enum class EType : uint8
{
	Type1,
	Type2
};

UCLASS()
class UNREALEXAMPLES_API USerializationExampleObject : public UObject
{
	GENERATED_BODY()

public:
	USerializationExampleObject() {}


	UPROPERTY(EditAnywhere, Category = "Voice Characteristics")
		EType Language;

	UPROPERTY(EditAnywhere, Category = "Conversion")
		FString SaveAssetTo;

	// ���� ������ ������ Serialize �� ���� Read/Write ������ ������ ���� �� �ִ�.
	// ���� ��θ� �����ϴ� ���·� �����ϰų�,
	// Config�� �����ϴ� ���·� �����ϴ°��� ����.
	//UPROPERTY(EditAnywhere, Category = "Conversion")
	//	USkeleton* TargetSkeleton;

	UPROPERTY(EditAnywhere, Category = "Conversion")
		int32 ImportAudioNumber;

	UPROPERTY(EditAnywhere, Category = "Conversion")
		bool bInsertPlaySoundNotify;

	UPROPERTY(EditAnywhere, Category = "FilesToDump")
		bool bPhoneme;

	UPROPERTY(EditAnywhere, Category = "FilesToDump")
		bool bAnimClip;

	UPROPERTY(EditAnywhere, Category = "FilesToDump")
		bool bConversionLog;
};