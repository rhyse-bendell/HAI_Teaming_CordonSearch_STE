// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "SIOJConvert.h"
#include <chrono>
#include "CsvOutputBPLibrary.generated.h"

UCLASS()
class UCsvOutputBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:


	/**
	* Converts an Unreal Struct into a Csv Header
	*
	* @param AnyStruct		The struct to convert
	* @return				String header
	*/
	UFUNCTION(BlueprintPure, CustomThunk, Category = "CsvOutput", meta = (CustomStructureParam = "AnyStruct"))
		static FString StructToCsvHeader(UProperty* AnyStruct);

	DECLARE_FUNCTION(execStructToCsvHeader)
	{
		Stack.Step(Stack.Object, NULL);
		UStructProperty* StructProperty = ExactCast<UStructProperty>(Stack.MostRecentProperty);
		void* StructPtr = Stack.MostRecentPropertyAddress;

		P_FINISH;
		TSharedPtr<FJsonObject> JsonObject = USIOJConvert::ToJsonObject(StructProperty->Struct, StructPtr, true);
		*(FString*)RESULT_PARAM = InternalStructToCsvHeader(JsonObject);
	}

	/**
	* Converts an Unreal Struct into a Csv Value Row
	*
	* @param AnyStruct		The struct to convert
	* @return				String header
	*/
	UFUNCTION(BlueprintPure, CustomThunk, Category = "CsvOutput", meta = (CustomStructureParam = "AnyStruct"))
		static FString StructToCsvRow(UProperty* AnyStruct);

	DECLARE_FUNCTION(execStructToCsvRow)
	{
		Stack.Step(Stack.Object, NULL);
		UStructProperty* StructProperty = ExactCast<UStructProperty>(Stack.MostRecentProperty);
		void* StructPtr = Stack.MostRecentPropertyAddress;

		P_FINISH;
		TSharedPtr<FJsonObject> JsonObject = USIOJConvert::ToJsonObject(StructProperty->Struct, StructPtr, true);
		*(FString*)RESULT_PARAM = InternalStructToCsvRow(JsonObject);
	}

	UFUNCTION(BlueprintPure, Category = Time)
		static int GetChronoMilliseconds()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}

	static FString InternalStructToCsvHeader(TSharedPtr<FJsonObject> JsonObjectPtr);

	static FString InternalStructToCsvRow(TSharedPtr<FJsonObject> JsonObjectPtr);
};
