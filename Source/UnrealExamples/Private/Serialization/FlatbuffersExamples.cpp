// Fill out your copyright notice in the Description page of Project Settings.


#include "Serialization/FlatbuffersExamples.h"

void FlatbuffersExamples::AllExamples()
{

	FlatbuffersExample();
}

void FlatbuffersExamples::FlatbuffersExample()
{
	TArray<uint8> Payload;

	FlatbuffersUtils::Serialize(Payload);

	FMonsterData MonsterData;

	FlatbuffersUtils::Deserialize(Payload, MonsterData);
}