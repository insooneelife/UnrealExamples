#include "Memcached.h"
#include "Core.h"
#include "Modules/ModuleManager.h"

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <map>

#if PLATFORM_WINDOWS

#include "Windows/AllowWindowsPlatformTypes.h"
#include <WS2tcpip.h>
#include "libmemcached/memcached.hpp"
#include "Windows/HideWindowsPlatformTypes.h"

#elif
#include <WS2tcpip.h>
#include "libmemcached/memcached.hpp"
#endif


#define LOCTEXT_NAMESPACE "FMemcachedModule"


static FString ErrorToString(memcached_return_t InReturn)
{
	const char* error = memcached_strerror(NULL, InReturn);
	FString ErrorText = ANSI_TO_TCHAR(error);
	return ErrorText;
}


const uint32 FMemcached::FlagInteger = 0x01 << 1;
const uint32 FMemcached::FlagText = 0x01 << 4;
const uint32 FMemcached::FlagBoolean = 0x01 << 5;
const uint32 FMemcached::FlagFloat = 0x01 << 6;
const uint32 FMemcached::FlagJson = 0x01 << 7;
const uint32 FMemcached::FlagStream = 0x01 << 8;


bool FMemcached::Start(const FString& InHostName, int InPort)
{
	FScopeLock Lock(&CriticalSection);
	WSADATA wsaData;

	// Initialize Winsock
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		UE_LOG(LogMemcached, Error, TEXT("WSAStartup failed: %d"), iResult);
		return false;
	}

	memcached_return_t rc;
	Memc = memcached(NULL, 0);

	if (Memc)
	{
		rc = memcached_server_add(Memc, TCHAR_TO_UTF8(*InHostName), InPort);

		if (rc != MEMCACHED_SUCCESS)
		{
			UE_LOG(LogMemcached, Error, TEXT("Memcached connect failed.  Error : %s"), *ErrorToString(rc));
			return false;
		}
		else
		{
			return true;
		}
	}

	return false;
}


void FMemcached::Destroy()
{
	FScopeLock Lock(&CriticalSection);

	WSACleanup();

	if (Memc != nullptr)
	{		
		//memcached_flush(Memc, 0);
		memcached_free(Memc);
		Memc = nullptr;
	}
}

bool FMemcached::Get(const FString& InKey, FString& OutString, uint32& OutFlags)
{
	SCOPE_CYCLE_COUNTER(STAT_MemcachedGet);

	size_t value_length = 0;
	memcached_return_t rc;

	FTCHARToUTF8 Utf8Key(*InKey);
	int32 KeyLenth = Utf8Key.Length();

	char* value;
	{
		FScopeLock Lock(&CriticalSection);
		value = memcached_get(Memc, Utf8Key.Get(), KeyLenth, &value_length, &OutFlags, &rc);
	}

	if (value != NULL && rc == MEMCACHED_SUCCESS)
	{
		OutString = UTF8_TO_TCHAR(value);

		//UE_LOG(LogMemcached, Log, TEXT("Memcached get success.  Key : %s  Out : %s"),
		//	*InKey, *OutString);

		free(value);
		return true;
	}

	else if (rc == MEMCACHED_NOTFOUND)
	{
		return false;
	}
	else
	{
		UE_LOG(LogMemcached, Error, TEXT("Memcached get failed.  Key : %s  Error : %s"),
			*InKey, *ErrorToString(rc));
		return false;
	}
}


bool FMemcached::SetJson(const FString& InKey, const FString& InJson)
{
	SCOPE_CYCLE_COUNTER(STAT_MemcachedSet);

	FTCHARToUTF8 Utf8Key(*InKey);
	int32 KeyLenth = Utf8Key.Length();

	FTCHARToUTF8 Utf8Json(*InJson);
	int32 JsonLenth = Utf8Json.Length();
	memcached_return_t rc;

	// 0 means no expire.
	{
		FScopeLock Lock(&CriticalSection);
		time_t ExpTime = 0;
		rc = memcached_set(Memc, Utf8Key.Get(), KeyLenth, Utf8Json.Get(), JsonLenth, ExpTime, FlagJson);
	}

	if (rc == MEMCACHED_SUCCESS)
	{
		return true;
	}

	return false;
}

bool FMemcached::Set(const FString& InKey, const FString& InData, uint32 InFlags)
{
	SCOPE_CYCLE_COUNTER(STAT_MemcachedSet);

	FTCHARToUTF8 Utf8Key(*InKey);
	int32 KeyLenth = Utf8Key.Length();

	FTCHARToUTF8 Utf8Data(*InData);
	int32 DataLenth = Utf8Data.Length();
	memcached_return_t rc;

	// 0 means no expire.
	{
		FScopeLock Lock(&CriticalSection);
		time_t ExpTime = 0;
		rc = memcached_set(Memc, Utf8Key.Get(), KeyLenth, Utf8Data.Get(), DataLenth, ExpTime, InFlags);
	}

	if (rc == MEMCACHED_SUCCESS)
	{
		return true;
	}

	//UE_LOG(LogMemcached, Error, TEXT("Memcached set failed.  Error : %s"), *ErrorToString(rc));

	return false;
}




#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMemcachedModule, Memcached)