// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "CsvOutputBPLibrary.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "CsvOutput.h"

UCsvOutputBPLibrary::UCsvOutputBPLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FString UCsvOutputBPLibrary::InternalStructToCsvHeader(TSharedPtr<FJsonObject> JsonObjectPtr)
{
	TArray<FString> Keys;
	JsonObjectPtr->Values.GetKeys(Keys);
	Keys.Sort();
	return FString::Join(Keys, TEXT(","));
}

FString UCsvOutputBPLibrary::InternalStructToCsvRow(TSharedPtr<FJsonObject> JsonObjectPtr)
{
	TArray<FString> CsvValues;
	auto JsonValues = JsonObjectPtr->Values;
	JsonValues.KeySort([](FString A, FString B) {
		return A < B;
	});
	for (auto JsonValue : JsonValues)
	{
		CsvValues.Add(JsonValue.Value.Get()->AsString());
	}
	return FString::Join(CsvValues, TEXT(","));
}
