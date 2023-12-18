#include "Casting/CastingExamples.h"


void CastingExamples::BaseExample()
{
	UE_LOG(LogTemp, Log, TEXT("BaseExample  Begin"));

	// 1. static cast�� �̿��� down casting
	{
		TSharedPtr<FNativeBaseData> Base = MakeShareable(new FNativeDerivedData());
		TSharedPtr<FNativeDerivedData> Derived = StaticCastSharedPtr<FNativeDerivedData>(Base);

		check(Derived != nullptr);
		check(Derived.IsValid());
	}

	// 2. static cast�� �̿��� down casting�� ������ ����
	{
		TSharedPtr<FUnrealBaseData> Base = MakeShareable(new FUnrealDerivedData());
		TSharedPtr<FUnrealDerivedData2> Derived = StaticCastSharedPtr<FUnrealDerivedData2>(Base);

		// nullptr�� �������� ����
		check(Derived == nullptr);
		check(!Derived.IsValid());
	}

	// 3. Cast �Լ��� �̿��� down casting
	{
		UUnrealBaseObject* Base = NewObject<UUnrealDerivedObject>();
		UUnrealDerivedObject* Derived = Cast<UUnrealDerivedObject>(Base);

		check(Derived != nullptr);
	}

	// 4. Cast �Լ��� ���������� ������ ����.
	{
		UUnrealBaseObject* Base = NewObject<UUnrealDerivedObject>();
		UUnrealDerivedObject2* Derived = Cast<UUnrealDerivedObject2>(Base);

		check(Derived == nullptr);
	}

}