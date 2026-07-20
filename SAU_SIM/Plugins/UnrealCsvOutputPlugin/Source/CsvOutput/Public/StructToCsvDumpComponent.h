// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CsvOutputBPLibrary.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "StructToCsvDumpComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UStructToCsvDumpComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStructToCsvDumpComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString FileDirectory = FString("/");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString FileName = FString("Default.csv");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> LineBuffer;

	UFUNCTION(BlueprintCallable, CustomThunk, Category = "CsvOutput", meta = (CustomStructureParam = "AnyStruct"))
		void InitialiseStructToCsvFileHandle(UProperty* AnyStruct);

	DECLARE_FUNCTION(execInitialiseStructToCsvFileHandle)
	{
		Stack.Step(Stack.Object, NULL);
		UStructProperty* StructProperty = ExactCast<UStructProperty>(Stack.MostRecentProperty);
		void* StructPtr = Stack.MostRecentPropertyAddress;

		P_FINISH;
		TSharedPtr<FJsonObject> JsonObject = USIOJConvert::ToJsonObject(StructProperty->Struct, StructPtr, true);
		P_THIS->LineBuffer.Add(UCsvOutputBPLibrary::InternalStructToCsvHeader(JsonObject));
	}


	UFUNCTION(BlueprintCallable, CustomThunk, Category = "CsvOutput", meta = (CustomStructureParam = "AnyStruct"))
		void AddRowToStructToCsvFileHandle(UProperty* AnyStruct);

	DECLARE_FUNCTION(execAddRowToStructToCsvFileHandle)
	{
		Stack.Step(Stack.Object, NULL);
		UStructProperty* StructProperty = ExactCast<UStructProperty>(Stack.MostRecentProperty);
		void* StructPtr = Stack.MostRecentPropertyAddress;

		P_FINISH;
		TSharedPtr<FJsonObject> JsonObject = USIOJConvert::ToJsonObject(StructProperty->Struct, StructPtr, true);

		P_THIS->LineBuffer.Add(UCsvOutputBPLibrary::InternalStructToCsvRow(JsonObject));
	}

	UFUNCTION(BlueprintCallable)
		void FlushBufferToFile()
	{
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

		if (PlatformFile.CreateDirectoryTree(*FileDirectory))
		{
			FString AbsoluteFilePath = FileDirectory + "/" + FileName;
			FFileHelper::SaveStringToFile(FString::Join(LineBuffer, TEXT("\n")), *AbsoluteFilePath);
		}
	}
};
