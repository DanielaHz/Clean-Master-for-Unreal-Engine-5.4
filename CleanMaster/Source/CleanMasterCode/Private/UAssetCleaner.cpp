// Fill out your copyright notice in the Description page of Project Settings.

#include "UAssetCleaner.h"
#include "../Public/UAssetCleaner.h"

#include "../../../../../../../../../Program Files/Epic Games/UE_5.4/Engine/Source/Runtime/AssetRegistry/Public/AssetRegistry/AssetRegistryModule.h"
#include "../../../../../../../../../Program Files/Epic Games/UE_5.4/Engine/Source/Runtime/AssetRegistry/Public/AssetRegistry/IAssetRegistry.h"
#include "../../../../../../../../../Program Files/Epic Games/UE_5.4/Engine/Source/Runtime/Core/Public/CoreGlobals.h"
#include "../../../../../../../../../Program Files/Epic Games/UE_5.4/Engine/Source/Runtime/CoreUObject/Public/AssetRegistry/ARFilter.h"
#include "../../../../../../../../../Program Files/Epic Games/UE_5.4/Engine/Source/Runtime/CoreUObject/Public/UObject/TopLevelAssetPath.h"
#include "ObjectTools.h"

TArray<FAssetData> UUAssetCleaner::FindAllUassets()
{
	// Load the AssetRegistry module to scan the /Game path and retrieve all UAssets available in the project
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	AssetRegistry.SearchAllAssets(true); // Force the search of assets 
	AssetRegistry.ScanPathsSynchronous({"/Game"}, true); // Scane all uassets in the content folder exclusive

	TArray<FAssetData> AssetData;
	const FString Path = "/Game";
	AssetRegistry.GetAssetsByPath(FName(*Path), AssetData, true);  // Find the assets in a recursive way in the whole content folder
	
	return AssetData;
}

void UUAssetCleaner::FindAllFiles()
{
	// Function to search for assets using the file manager. Maybe can be very useful when im attempting to delete the not uassets.
	FString ContentFolderPath = FPaths::ProjectContentDir();
	TArray<FString> Files;
	IFileManager& FileManager = IFileManager::Get();

	FileManager.FindFilesRecursive(Files, *ContentFolderPath, TEXT("*.*"), true, false, false);
	for (FString& File : Files)
	{
		// ignoring External actors 
		if (!File.Contains(TEXT("__ExternalActors__")))
		{
			UE_LOG(LogTemp, Display, TEXT("FindAllFiles: %s"), *File);
		}
	}
}

TArray<FAssetData> UUAssetCleaner::FindUnusedAsset()
{
	TArray<FAssetData> AllAssets = FindAllUassets();
	TArray<FAssetData> UnusedAssets;

	if (AllAssets.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No assets found."));
		return UnusedAssets;
	}
	
	// Use the AssetRegistry to find hard referencers and determine which assets are unused
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	for (const FAssetData& Asset : AllAssets)
	{
		// Ignoring level intern references. 
		if (Asset.ObjectPath.ToString().Contains(TEXT(":PersistentLevel")))
		{
			continue;
		}
		
		// Ignoring levels o worlds
		if (Asset.GetClass()->IsChildOf(UWorld::StaticClass()) || Asset.GetClass()->IsChildOf(ULevel::StaticClass()))
		{
			continue;
		}
		
		TArray<FName> Referencers;

		// Validate if the the asset have any referencers.
		bool bSuccess = AssetRegistry.GetReferencers(Asset.PackageName, Referencers,
			UE::AssetRegistry::EDependencyCategory::Package,
			UE::AssetRegistry::EDependencyQuery::Hard);

		if (!bSuccess)
		{
			UE_LOG(LogTemp, Warning, TEXT("No referencers found for asset %s"), *Asset.PackageName.ToString());
			continue;
		}
		
		if (Referencers.Num() == 0)
		{
			UnusedAssets.Add(Asset);
			UE_LOG(LogTemp, Display, TEXT("Unused asset detected: %s"), *Asset.ObjectPath.ToString());
		}
	}
	return UnusedAssets;
}

void UUAssetCleaner::PrintingUnusedAssets(TArray<FAssetData>& UnusedAssets)
{
	for (const FAssetData& UnusedAsset : UnusedAssets)
	{
		UE_LOG(LogTemp, Display,
			TEXT("Unused Asset - Name: %s, Path: %s, Class: %s"),
			*UnusedAsset.AssetName.ToString(),
			*UnusedAsset.ObjectPath.ToString(),
			*UnusedAsset.GetClass()->GetName());
	}
}

void UUAssetCleaner::BackupAssets()
{
	// Define the folder of the origin
	FString SourceDir = FPaths::ProjectContentDir();

	// Define the backup folder
	FString BackupDir = FPaths::Combine(FPaths::ProjectDir(), TEXT("Saved/Logs"));

	// Backup function
	IFileManager& FileManager = IFileManager::Get();
	FileManager.Copy(*BackupDir, *SourceDir, true, true);
	UE_LOG(LogTemp, Display, TEXT("Backup completed successfully."));
}

int32 UUAssetCleaner::DeleteUnusedAsset(TArray<FAssetData>& UnusedAssets)
{
	TArray<UObject*> AssetsToDelete;
	for (const FAssetData& Asset : UnusedAssets)
	{
		const FString ObjectPath = Asset.GetObjectPathString();
		UObject* LoadedAsset = LoadObject<UObject>(nullptr, *ObjectPath);

		if (LoadedAsset)
		{
			AssetsToDelete.Add(LoadedAsset);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load object: %s"), *ObjectPath);
		}
	}

	UE_LOG(LogTemp, Display, TEXT("Deleting %d assets from Content Browser"), AssetsToDelete.Num());

	// Force deletion of the loaded assets from the editor (bypasses confirmation prompts)
	int result = ObjectTools::DeleteObjectsUnchecked(AssetsToDelete);

	if (result !=0)
	{
		return false;
	}
	
	// Clean the array
	UnusedAssets.Empty();

	//  Perform a full garbage collection to remove deleted objects from memory
	CollectGarbage(RF_NoFlags);

	PrintingUnusedAssets(UnusedAssets);

	return 0; // let know the UI the process was complete successfully
}