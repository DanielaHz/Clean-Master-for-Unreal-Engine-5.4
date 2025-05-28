// Fill out your copyright notice in the Description page of Project Settings.

#include "UAssetDataEntry.h"
#include "../Public/UAssetDataEntry.h"
#include "Components/TextBlock.h"
#include "CleanMasterCode/Public/UAssetsData.h"

/*
 *This class contain the data entry for the view list widget in the clean master widget
 */

void UUAssetDataEntry::NativeOnListItemObjectSet(UObject* ListItem)
{
	if (UUAssetsData* AssetData = Cast<UUAssetsData>(ListItem))
	{
		if (AssetNameText)
			AssetNameText->SetText(FText::FromName(AssetData->AssetName));

		if (AssetPathText)
			AssetPathText->SetText(FText::FromString(AssetData->AssetPath));

		if (AssetTypeText)
			AssetTypeText->SetText(FText::FromString(AssetData->AssetType));

		if (SizeText)
		{
			SizeText->SetText(FText::AsNumber(AssetData->AssetSize));
		}

		if (LastModifiedText)
		{
			FString DateString = AssetData->AssetLastModified.ToString();
			LastModifiedText->SetText(FText::FromString(DateString));
		}
	}
}
