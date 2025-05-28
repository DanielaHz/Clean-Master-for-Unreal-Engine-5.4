#pragma once

#include <filesystem>
#include <iostream>

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Misc/Paths.h"
#include "HAL/FileManagerGeneric.h"
#include "HAL/FileManager.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Misc/DateTime.h"
#include "Misc/OutputDeviceNull.h"
#include "Engine/World.h"
#include "Engine/Level.h"
#include "FileHelpers.h"
#include "UAssetCleaner.generated.h"

/*
 * This class contain all the logic to clean the assets
 */
UCLASS()
class CLEANMASTERCODE_API UUAssetCleaner : public UObject
{
	GENERATED_BODY()

public:
	TArray<FAssetData> FindAllUassets(); // All the assets in the content browser using the assetRegistry module
	void FindAllFiles(); // Looking for all the assets in the content directory
	TArray<FAssetData> FindUnusedAsset(); // This function checks if assets are referenced anywhere in the code. If references == 0, the asset can be safely removed.
	void PrintingUnusedAssets(TArray<FAssetData> &UnusedAssets); // Just to check if the right assets were added.
	void BackupAssets(); //Avoid conflicts if something goes wrong when I'm deleting the assets in the project I will use this function to create a backup of the files in the content browser 
	int32 DeleteUnusedAsset(TArray<FAssetData>& UnusedAssets); //This function deletes the unused assets in the project using the module ObjectTools.
	void RestoreAssetsFromBackup(); // function to tell to unreal where to scan if the deleting assets process failed
};
