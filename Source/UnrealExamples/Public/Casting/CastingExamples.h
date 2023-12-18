#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Animation/Skeleton.h"
#include "CastingExamples.generated.h"

struct FNativeBaseData
{
	int Value;
};

struct FNativeDerivedData : public FNativeBaseData
{
};

struct FNativeDerivedData2 : public FNativeBaseData
{
};


USTRUCT()
struct FUnrealBaseData
{
	GENERATED_USTRUCT_BODY()

public:
	int Value;
};


USTRUCT()
struct FUnrealDerivedData : public FUnrealBaseData
{
	GENERATED_USTRUCT_BODY()

public:
	int Value;
};

USTRUCT()
struct FUnrealDerivedData2 : public FUnrealBaseData
{
	GENERATED_USTRUCT_BODY()

public:
	int Value;
};

UCLASS()
class UUnrealBaseObject : public UObject
{
	GENERATED_BODY()

public:

};

UCLASS()
class UUnrealDerivedObject : public UUnrealBaseObject
{
	GENERATED_BODY()

public:

};


UCLASS()
class UUnrealDerivedObject2 : public UUnrealBaseObject
{
	GENERATED_BODY()

public:

};


class UNREALEXAMPLES_API CastingExamples
{
public:

	static void BaseExample();

};



UCLASS()
class UNREALEXAMPLES_API UCastingExampleObject : public UObject
{
	GENERATED_BODY()

public:

};