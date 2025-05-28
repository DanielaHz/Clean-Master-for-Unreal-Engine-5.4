// Copyright Epic Games, Inc. All Rights Reserved.
// The following Unreal Engine class is based on the project cited below. I generated the modules from scratch, but the content follows the base structure of the plugin template created by Damian Nowakowski.
// Modified from:
// Damian Nowakowski (June 4, 2024). UE4EditorPluginTemplate [online].
// [Accessed 2016]. Available from: <https://github.com/zompi2/UE4EditorPluginTemplate>

#pragma once

#include "CoreMinimal.h"
#include "CleanMasterCode/Public/UAssetCleaner.h"
#include "Components/Button.h"
#include "Components/ListView.h"
#include "Editor/Blutility/Classes/EditorUtilityWidget.h"
#include "UObject/NoExportTypes.h"
#include "Components/CanvasPanel.h"
#include "MediaPlayer.h"
#include "MediaTexture.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Misc/Guid.h"
#include "UAssetsData.h"
#include "ISettingsModule.h"
#include "Modules/ModuleManager.h"
#include "Framework/Commands/GenericCommands.h"
#include "CleanMasterWidget.generated.h"

/*
 * This class contain all the logic and bind of the clean master widget
 */
UCLASS()
class CLEANMASTER_API UCleanMasterWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* RootCanvas;
	
	// Buttons definition and bind
	UPROPERTY(meta = (BindWidget))
	UButton* FindButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* CleanButton;

	UPROPERTY(meta = (BindWidget))
	UButton* RefreshButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SettingsButton;

	UPROPERTY(meta = (BindWidget))
	UButton* PackageButton;

	UPROPERTY()
	UUserWidget* PopupWidget;

	UPROPERTY()
	UUserWidget* SuccessNotification;
	
	// List view
	UPROPERTY(meta = (BindWidget))
	UListView* AssetsListView;
	
	// Notification widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Plugin Editor Widget")
	TSubclassOf<UUserWidget> BP_BackupSuccessNotification;

	// Notification fail cleaning
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Plugin Editor Widget")
	TSubclassOf<UUserWidget> BP_CleaningFailNotification;

	// Notification success cleaning
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Plugin Editor Widget")
	TSubclassOf<UUserWidget> BP_CleaningSuccessNotification;

	// Popup window widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Plugin Editor Widget")
	TSubclassOf<UUserWidget> BP_CleanProcessPopup;

	// Notification not assets to clean 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Plugin Editor Widget")
	TSubclassOf<UUserWidget> BP_UnusedAssetsEmptyNotification;
	
	// Notification Assets refreshed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Plugin Editor Widget")
	TSubclassOf<UUserWidget> BP_AssetsRefreshedNotification;

	// Notification Section On Development
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Plugin Editor Widget")
	TSubclassOf<UUserWidget> BP_SectionOnDevelopment;
	// Defining the native construct for the clean master widget
	void NativeConstruct() override;

	// Functions when the buttons are pressed
	UFUNCTION()
	void FindButtonPressed();

	UFUNCTION()
	void CleanButtonPressed();

	UFUNCTION()
	void RefreshButtonPressed();

	UFUNCTION()
	void SettingsButtonPressed();

	UFUNCTION()
	void PackageButtonPressed();
	
	// Helper functions to the main buttons in the widget
	UFUNCTION()
	void CallBackupSuccessNotification(); // this notification shows the user when the backup is success

	UFUNCTION()
	void CallCleanProcessPopup(); // this window shows the user when the clean process stars

	UFUNCTION()
	void CallCleaningSuccessNotification(); // this funciton shows the notification message of the cleaning process was successful

	UFUNCTION()
	void CallCleaningFailNotification(); // this function shows the notification message of the cleaning process wasn't successful.

	UFUNCTION()
	void CallUnussedAssetsNotification(); // This function shows the notification message when there are not uassets to clean.

	UFUNCTION()
	void CallAssetsRefreshed(); // This function shows the notification of the assets are refreshed.
	
	UFUNCTION()
	void CloseCleanProcessPopup(); // This function close the popup window
	
	UFUNCTION()
	void PopulateListView(); // this function will bind the data of the unused assets with the view list widget
	
	UFUNCTION()
	void ConvertUassetsToUobjects(); // this function converts the data of the uassets in uobject for the view list widget

	UFUNCTION()
	float GetAssetSize(FAssetData data); // this function transform the data of the size to UObject for the view list widget

	UFUNCTION()
	FDateTime GetAssetLastModified(FAssetData data); // this function transform the data of the date to UObject for the view list widget

	UFUNCTION()
	void OpenPackagingSettingsWindow(); // this function is to open the window with the same name when the packing buttons was clicked

private:
	UUAssetCleaner* cleaner; 
	TArray<FAssetData> unusedAssetsArray = cleaner->FindUnusedAsset();
	TArray<UObject*> unusedAssetsArrayObjects; //This array is to store the data converted to UObject fot the view list widget
};
