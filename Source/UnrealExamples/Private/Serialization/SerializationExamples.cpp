// Fill out your copyright notice in the Description page of Project Settings.


#include "Serialization/SerializationExamples.h"
#include "Serialization/MemoryWriter.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/ArrayReader.h"
#include "Serialization/ArrayWriter.h"

#include "HAL/FileManager.h"

void SerializationExamples::AllExamples()
{
	BaseExample();

	StructExample();

	ObjectExample();

	FileExample();

	MemoryAndFileExample();

	ArrayAchiveExample();

	FlatbuffersExample();
}

void SerializationExamples::BaseExample()
{
	UE_LOG(LogTemp, Log, TEXT("BaseExample  Begin"));

	// write data to memory
	TArray<uint8> BufferArray;
	FMemoryWriter Writer(BufferArray);

	{
		uint32 A = 15;
		uint32 B = 75;
		FString C = TEXT("blabla");

		Writer << A;
		Writer << B;
		Writer << C;
		UE_LOG(LogTemp, Log, TEXT("BufferArray Num : %d"), BufferArray.Num());
	}

	// read data from memory
	FMemoryReader Reader(BufferArray);

	{
		uint32 A;
		uint32 B;
		FString C;

		Reader << A;
		Reader << B;
		Reader << C;

		UE_LOG(LogTemp, Log, TEXT("A : %u  B : %u  C : %s"), A, B, *C);
	}


	UE_LOG(LogTemp, Log, TEXT("BaseExample  End"));
}

void SerializationExamples::StructExample()
{
	UE_LOG(LogTemp, Log, TEXT("StructSerializeExample  Begin"));

	TArray<uint8> BufferArray;
	FMemoryWriter Writer(BufferArray);

	{
		SerializationExampleData Data;
		Data.A = 99;
		Data.B = 13;
		Data.C = TEXT("blabla");

		Writer << Data;
		UE_LOG(LogTemp, Log, TEXT("BufferArray Num : %d"), BufferArray.Num());
	}


	FMemoryReader Reader(BufferArray);

	{
		SerializationExampleData Data;
		Reader << Data;
		UE_LOG(LogTemp, Log, TEXT("Data A : %u  Data B : %u  Data C : %s"), Data.A, Data.B, *Data.C);
	}


	{
		// need to seek to use again
		Reader.Seek(0);
		SerializationExampleData Data;
		Reader << Data;
		UE_LOG(LogTemp, Log, TEXT("Data A : %u  Data B : %u  Data C : %s"), Data.A, Data.B, *Data.C);
	}


	{
		// this will not work properly
		SerializationExampleData Data;
		Reader << Data;
		UE_LOG(LogTemp, Log, TEXT("Data A : %u  Data B : %u  Data C : %s  (Error Case)"), Data.A, Data.B, *Data.C);
	}

	{
		// once reader offset overflowed, it doesn't work properly even if seek offset
		Reader.Seek(0);
		SerializationExampleData Data;
		Reader << Data;
		UE_LOG(LogTemp, Log, TEXT("Data A : %u  Data B : %u  Data C : %s  (Error Case)"), Data.A, Data.B, *Data.C);
	}

	// safe example - just create archive object in brace
	{
		FMemoryReader MemReader(BufferArray);
		SerializationExampleData Data;
		MemReader << Data;
		UE_LOG(LogTemp, Log, TEXT("Data A : %u  Data B : %u  Data C : %s"), Data.A, Data.B, *Data.C);
	}

	UE_LOG(LogTemp, Log, TEXT("StructSerializeExample  End"));
}

void SerializationExamples::ObjectExample()
{
	UE_LOG(LogTemp, Log, TEXT("ObjectExample  Begin"));
	TArray<uint8> BufferArray;

	// write
	{
		FMemoryWriter Writer(BufferArray);

		USerializationExampleObject* Obj = NewObject<USerializationExampleObject>();
		Obj->Language = EType::Type2;
		Obj->SaveAssetTo = TEXT("Maybe/Some/Directory/Path/Here");
		Obj->ImportAudioNumber = 33;
		Obj->bInsertPlaySoundNotify = true;
		Obj->bPhoneme = false;
		Obj->bAnimClip = true;
		Obj->bConversionLog = false;

		Obj->Serialize(Writer);
		UE_LOG(LogTemp, Log, TEXT("BufferArray Num : %d"), BufferArray.Num());
	}

	// read
	{
		FMemoryReader Reader(BufferArray);
		USerializationExampleObject* Obj = NewObject<USerializationExampleObject>();

		Obj->Serialize(Reader);

		UE_LOG(LogTemp, Log, TEXT("Readed Data : %d  %s  %d  %d  %d  %d  %d"),
			Obj->Language,
			*Obj->SaveAssetTo,
			Obj->ImportAudioNumber,
			Obj->bInsertPlaySoundNotify,
			Obj->bPhoneme,
			Obj->bAnimClip,
			Obj->bConversionLog);
	}


	UE_LOG(LogTemp, Log, TEXT("ObjectExample  End"));
}


void SerializationExamples::FileExample()
{
	UE_LOG(LogTemp, Log, TEXT("FileExample  Begin"));

	FString FullPath = FString::Printf(TEXT("%s%s"), *FPaths::ProjectSavedDir(), TEXT("save.txt"));

	// write
	FArchive* FileWriter = IFileManager::Get().CreateFileWriter(*FullPath);
	if (FileWriter)
	{
		FString T1 = FString(TEXT("AAA"));
		FString T2 = FString(TEXT("BBB"));
		*FileWriter << T1;
		*FileWriter << T2;
		FileWriter->Close();
		delete FileWriter;
		FileWriter = NULL;
	}

	// read (can be used with TSharedPtr, which deletes memory by-self)
	TSharedPtr<FArchive> FileReader = MakeShareable(IFileManager::Get().CreateFileReader(*FullPath));
	if (FileReader.IsValid())
	{
		FString Temp1;
		FString Temp2;

		*FileReader.Get() << Temp1;
		*FileReader.Get() << Temp2;

		FileReader->Close();
		UE_LOG(LogTemp, Log, TEXT("%s %s"), *Temp1, *Temp2);
	}

	UE_LOG(LogTemp, Log, TEXT("FileExample  End"));
}

void SerializationExamples::MemoryAndFileExample()
{
	UE_LOG(LogTemp, Log, TEXT("MemoryAndFileExample  Begin"));

	FString FullPath = FString::Printf(TEXT("%s%s"), *FPaths::ProjectSavedDir(), TEXT("save.txt"));

	// write (data -> memory -> file)
	TSharedPtr<FArchive> FileWriter = MakeShareable(IFileManager::Get().CreateFileWriter(*FullPath));
	if (FileWriter.IsValid())
	{
		TArray<uint8> Buffer;
		FMemoryWriter MemoryWriter(Buffer);

		SerializationExampleData Data;
		Data.A = 99;
		Data.B = 13;
		Data.C = TEXT("blabla");

		MemoryWriter << Data;

		*FileWriter << Buffer;
		FileWriter->Close();
	}


	// read (file -> memory -> data)
	TSharedPtr<FArchive> FileReader = MakeShareable(IFileManager::Get().CreateFileReader(*FullPath));
	if (FileReader.IsValid())
	{
		TArray<uint8> Buffer;
		*FileReader << Buffer;

		FMemoryReader MemoryReader(Buffer);

		SerializationExampleData Data;
		MemoryReader << Data;
		FileReader->Close();


		UE_LOG(LogTemp, Log, TEXT("Data A : %u  Data B : %u  Data C : %s"), Data.A, Data.B, *Data.C);
	}

	UE_LOG(LogTemp, Log, TEXT("MemoryAndFileExample  End"));
}

void SerializationExamples::ArrayAchiveExample()
{
	UE_LOG(LogTemp, Log, TEXT("ArrayAchiveExample  Begin"));

	// array writer is has both array & writer functions.
	// it can be used without buffer array. (actually it has one inside.)
	FArrayWriter Writer;
	{
		uint32 A = 15;
		uint32 B = 75;
		FString C = TEXT("blabla");

		Writer << A;
		Writer << B;
		Writer << C;
		UE_LOG(LogTemp, Log, TEXT("Writer Num : %d"), Writer.Num());
	}

	// same as array writer
	FMemoryReader Reader(Writer);
	{
		uint32 A;
		uint32 B;
		FString C;

		Reader << A;
		Reader << B;
		Reader << C;

		UE_LOG(LogTemp, Log, TEXT("A : %u  B : %u  C : %s"), A, B, *C);
	}


	UE_LOG(LogTemp, Log, TEXT("ArrayAchiveExample  End"));
}



void SerializationExamples::FlatbuffersExample()
{
	TArray<uint8> Payload;

	FlatbuffersUtils::Serialize(Payload);

	FMonsterData MonsterData;

	FlatbuffersUtils::Deserialize(Payload, MonsterData);
}