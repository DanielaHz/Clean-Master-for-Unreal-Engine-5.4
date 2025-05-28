// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UAssetDataEntry.generated.h"

/*
 * This class contain the data entry for the view list widget in the clean master widget
 */
UCLASS()
class CLEANMASTER_API UUAssetDataEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	virtual void NativeOnListItemObjectSet(UObject* ListItem);

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AssetNameText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AssetPathText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AssetTypeText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SizeText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LastModifiedText;
};
