// Copyright NCSOFT, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Stats/Stats.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMemcached, Log, All);
DECLARE_STATS_GROUP(TEXT("Memcached"), STATGROUP_Memcached, STATCAT_Advanced);
DECLARE_CYCLE_STAT(TEXT("MemcachedGet"), STAT_MemcachedGet, STATGROUP_Memcached);
DECLARE_CYCLE_STAT(TEXT("MemcachedSet"), STAT_MemcachedSet, STATGROUP_Memcached);