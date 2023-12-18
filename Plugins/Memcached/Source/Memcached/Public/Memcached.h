#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "MemcachedCore.h"
#include "HAL/CriticalSection.h"

class MEMCACHED_API FMemcached
{
public:
	const static uint32 FlagInteger;
	const static uint32 FlagText;
	const static uint32 FlagBoolean;
	const static uint32 FlagFloat;
	const static uint32 FlagJson;
	const static uint32 FlagStream;

public:
	bool Start(const FString& InHostName, int InPort);
	void Destroy();
	bool Get(const FString& InKey, FString& OutString, uint32& OutFlags);
	bool SetJson(const FString& InKey, const FString& InJson);

private:
	bool Set(const FString& InKey, const FString& InData, uint32 InFlags);

private:
	FCriticalSection CriticalSection;

	struct memcached_st* Memc;
};


class MEMCACHED_API FMemcachedModule : public IModuleInterface
{
public:

	// IModuleInterface implementation
	virtual void StartupModule() override {}
	virtual void ShutdownModule() override {}
};