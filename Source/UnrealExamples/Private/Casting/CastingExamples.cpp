#include "Casting/CastingExamples.h"


void CastingExamples::BaseExample()
{
	UE_LOG(LogTemp, Log, TEXT("BaseExample  Begin"));

	// 1. static cast를 이용한 down casting
	{
		TSharedPtr<FNativeBaseData> Base = MakeShareable(new FNativeDerivedData());
		TSharedPtr<FNativeDerivedData> Derived = StaticCastSharedPtr<FNativeDerivedData>(Base);

		check(Derived != nullptr);
		check(Derived.IsValid());
	}

	// 2. static cast를 이용한 down casting의 안정성 문제
	{
		TSharedPtr<FUnrealBaseData> Base = MakeShareable(new FUnrealDerivedData());
		TSharedPtr<FUnrealDerivedData2> Derived = StaticCastSharedPtr<FUnrealDerivedData2>(Base);

		// nullptr을 감지하지 못함
		check(Derived == nullptr);
		check(!Derived.IsValid());
	}

	// 3. Cast 함수를 이용한 down casting
	{
		UUnrealBaseObject* Base = NewObject<UUnrealDerivedObject>();
		UUnrealDerivedObject* Derived = Cast<UUnrealDerivedObject>(Base);

		check(Derived != nullptr);
	}

	// 4. Cast 함수의 안정성에는 문제가 없음.
	{
		UUnrealBaseObject* Base = NewObject<UUnrealDerivedObject>();
		UUnrealDerivedObject2* Derived = Cast<UUnrealDerivedObject2>(Base);

		check(Derived == nullptr);
	}

}