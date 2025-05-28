#include "Misc/AutomationTest.h"
#include "UAssetCleaner.h"

/*
 * This class contain the unit test for the UAssetCleaner class 
 */

// Unit test declaration (" name of the teste" ,  Description fo the test, " flags to tell to unreal where we want to run this tests, in this case we want to run the test in the 
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTestFindAllUassets, "CleanMaster.UnitTest.FTestFindAllUassets",EAutomationTestFlags::EditorContext | EAutomationTestFlags::SmokeFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTestFindAllAssets, "CleanMaster.UnitTest.FTestFindAllAssets", EAutomationTestFlags::EditorContext | EAutomationTestFlags::SmokeFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTestFindUnusedAsset, "CleanMaster.UnitTest.FTestFindUnusedAsset", EAutomationTestFlags::EditorContext | EAutomationTestFlags::SmokeFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTestBackUpAssets, "CleanMaster.UnitTest.FTestBackUpAssets", EAutomationTestFlags::EditorContext | EAutomationTestFlags::SmokeFilter)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTestDeleteUnusedAsset, "CleanMaster.UnitTest.FTestDeleteUnusedAsset", EAutomationTestFlags::EditorContext | EAutomationTestFlags::SmokeFilter)

// Tests generated using AI Assitance

bool FTestFindAllUassets::RunTest(const FString& Parameters)
{
	UUAssetCleaner* AssetCleaner = NewObject<UUAssetCleaner>();

	TArray<FAssetData> AllAssets = AssetCleaner->FindAllUassets();
    
	if (AllAssets.Num() > 0)
	{
		TestTrue(TEXT("The first asset has a right path."), AllAssets[0].PackageName.ToString().Contains("/Game"));
	}
	else
	{
		AddError(TEXT("No assets found!"));
	}
	return true;
}

bool FTestFindAllAssets::RunTest(const FString& Parameters)
{
	// Create instance of your class
	UUAssetCleaner* Cleaner = NewObject<UUAssetCleaner>();
	if (!Cleaner)
	{
		AddError(TEXT("Failed to create UUAssetCleaner instance."));
		return false;
	}

	// Call the function — it should not crash
	Cleaner->FindAllFiles();

	// Since we can't capture UE_LOG easily or inspect the internal file list without refactoring,
	// we can only assert that execution completed.
	AddInfo(TEXT("FindAllFiles executed without crashing."));
	return true;
}

bool FTestFindUnusedAsset::RunTest(const FString& Parameter)
{
	// Create instance of the cleaner
	UUAssetCleaner* Cleaner = NewObject<UUAssetCleaner>();
	if (!Cleaner)
	{
		AddError(TEXT("Failed to create UUAssetCleaner instance."));
		return false;
	}

	// Call the function
	TArray<FAssetData> UnusedAssets = Cleaner->FindUnusedAsset();

	// Sanity check: function runs without crash
	AddInfo(FString::Printf(TEXT("FindUnusedAsset executed. Found %d unused assets."), UnusedAssets.Num()));

	// Optional: simple validation
	for (const FAssetData& Asset : UnusedAssets)
	{
		if (!Asset.IsValid())
		{
			AddWarning(FString::Printf(TEXT("Found an invalid asset in results: %s"), *Asset.ObjectPath.ToString()));
		}
		else
		{
			AddInfo(FString::Printf(TEXT("Unused: %s"), *Asset.ObjectPath.ToString()));
		}
	}
	return true;
}

bool FTestBackUpAssets::RunTest(const FString& Parameter)
{
	UUAssetCleaner* Cleaner = NewObject<UUAssetCleaner>();
	if (!Cleaner)
	{
		AddError(TEXT("Failed to create UUAssetCleaner instance."));
		return false;
	}

	FString BackupDir = FPaths::Combine(FPaths::ProjectDir(), TEXT("Saved/Logs"));
    
	// Cleanup before test (optional safety)
	if (FPaths::DirectoryExists(BackupDir))
	{
		IFileManager::Get().DeleteDirectory(*BackupDir, false, true);
	}

	// Run the method
	Cleaner->BackupAssets();

	// Verify the directory now exists
	if (!FPaths::DirectoryExists(BackupDir))
	{
		AddError(FString::Printf(TEXT("Backup directory does not exist after BackupAssets: %s"), *BackupDir));
		return false;
	}

	AddInfo(TEXT("BackupAssets completed and backup directory was created."));

	// Optional: Check if at least one file was copied
	TArray<FString> FoundFiles;
	IFileManager::Get().FindFilesRecursive(FoundFiles, *BackupDir, TEXT("*.*"), true, false);
    
	if (FoundFiles.Num() == 0)
	{
		AddWarning(TEXT("Backup directory exists but no files were copied."));
	}
	else
	{
		AddInfo(FString::Printf(TEXT("Found %d files in backup directory."), FoundFiles.Num()));
	}

	return true;
}

bool FTestDeleteUnusedAsset::RunTest(const FString& Parameter)
{
	UUAssetCleaner* Cleaner = NewObject<UUAssetCleaner>();
	if (!Cleaner)
	{
		AddError(TEXT("Failed to create UUAssetCleaner instance."));
		return false;
	}

	// Crear un objeto simulado
	UStaticMesh* TempMesh = NewObject<UStaticMesh>();
	if (!TempMesh)
	{
		AddError(TEXT("Failed to create a test UStaticMesh object."));
		return false;
	}

	// Crear FAssetData ficticio
	FAssetData DummyAsset(TempMesh);
	TArray<FAssetData> TestAssets;
	TestAssets.Add(DummyAsset);

	// Log del tamaño del array antes de la eliminación
	AddInfo(FString::Printf(TEXT("TestAssets array size before deletion: %d"), TestAssets.Num()));

	// Llamar a la función DeleteUnusedAsset
	int32 Result = Cleaner->DeleteUnusedAsset(TestAssets);

	// Verificar el resultado
	if (Result != 0)
	{
		AddError(FString::Printf(TEXT("DeleteUnusedAsset returned an error code: %d"), Result));
		return false;
	}

	// Log del tamaño del array después de la eliminación
	AddInfo(FString::Printf(TEXT("TestAssets array size after deletion: %d"), TestAssets.Num()));

	// Asegúrate de que el array esté vacío
	if (TestAssets.Num() != 0)
	{
		AddError(TEXT("UnusedAssets array was not cleared after deletion."));
	}
	else
	{
		AddInfo(TEXT("UnusedAssets array is cleared as expected."));
	}
	return true;
}
