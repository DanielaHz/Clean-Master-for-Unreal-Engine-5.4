// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UAssetsData.generated.h"

/**
 * This class contain the variables that we will show in the listview widget 
 */
UCLASS(Blueprintable)
class CLEANMASTERCODE_API UUAssetsData : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = "My Plugin Editor Widget")
	FName AssetName;

	UPROPERTY(BlueprintReadWrite, Category = "My Plugin Editor Widget")
	FString AssetPath;

	UPROPERTY(BlueprintReadWrite, Category = "My Plugin Editor Widget")
	FString AssetType;

	UPROPERTY(BlueprintReadWrite, Category = "My Plugin Editor Widget")
	float AssetSize;

	UPROPERTY(BlueprintReadWrite, Category = "My Plugin Editor Widget")
	FDateTime AssetLastModified;
};
