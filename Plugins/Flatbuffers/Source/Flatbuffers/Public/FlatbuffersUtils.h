#pragma once

#include "CoreMinimal.h"

struct FLATBUFFERS_API FMonsterData
{

};

class FLATBUFFERS_API FlatbuffersUtils
{
public:

	static void Serialize(TArray<uint8>& OutPayload);

	static void Deserialize(const TArray<uint8>& InPayload, FMonsterData& OutData);
};