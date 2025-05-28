// Fill out your copyright notice in the Description page of Project Settings.

#include "CleanMasterWidget.h"

#include "ContentBrowserModule.h"
#include "Editor.h"
#include "EditorUtilityWidgetComponents.h"
#include "IContentBrowserSingleton.h"
#include "Components/Button.h"
#include "Misc/LowLevelTestAdapter.h"
#include "Runtime/MediaAssets/Public/MediaPlayer.h"
#include "Runtime/MediaAssets/Public/MediaSource.h"
#include "Runtime/MediaAssets/Public/MediaTexture.h"
#include "Runtime/Serialization/Public/IStructDeserializerBackend.h"

void UCleanMasterWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (FindButton)
	{
		UE_LOG(LogTemp, Display, TEXT("Button Find Found"));
		FindButton->OnClicked.AddDynamic(this, &UCleanMasterWidget::FindButtonPressed);
	}
	
	else
	{
		{
			UE_LOG(LogTemp, Display, TEXT("Button Not Found"));
		}
	}

	if (CleanButton)
	{
		UE_LOG(LogTemp, Display, TEXT("Button Clean Found in the UI"));
		CleanButton->OnClicked.AddDynamic(this, &UCleanMasterWidget::CleanButtonPressed);
	}

	if (RefreshButton)
	{
		UE_LOG(LogTemp, Display, TEXT("Button Refresh Found in the UI"));
		RefreshButton->OnClicked.AddDynamic(this, &UCleanMasterWidget::RefreshButtonPressed);
	}

	if (SettingsButton)
	{
		UE_LOG(LogTemp, Display, TEXT("Button Settings Found in the UI"));
		SettingsButton->OnClicked.AddDynamic(this, &UCleanMasterWidget::SettingsButtonPressed);
	}

	if (PackageButton)
	{
		UE_LOG(LogTemp, Display, TEXT("Button Package Found in the UI"));
		PackageButton->OnClicked.AddDynamic(this, &UCleanMasterWidget::PackageButtonPressed);
	}

	if (AssetsListView) // here i should check if this is populating inmideatly when the widget appear, because I want to set list Items when I call findButton not automatically
	{
		AssetsListView->ClearListItems();
		UE_LOG(LogTemp, Display, TEXT("Calling to convert to uassets to Uobjects"))
	}
}

UFUNCTION()
void UCleanMasterWidget::FindButtonPressed()
{
	// Call the function on clean master how print all the unused assets -> terminal output
	UE_LOG(LogTemp, Display, TEXT("Printing what assets are unused in the content browser"));
	cleaner->PrintingUnusedAssets(unusedAssetsArray);
	
	// cleaning this array to avoid duplicates in the view list widget
	unusedAssetsArrayObjects.Empty();

	if (unusedAssetsArray.Num() > 0)
	{
		// In the UI I should show the list view of the content section and the data I want to show for any unused asset
		PopulateListView();
		UE_LOG(LogTemp, Display, TEXT("Passing the unusedAssetsArray to the Assets list view"));
		AssetsListView->SetListItems(unusedAssetsArrayObjects); // Array UObject with contain the data founded by the find function in the project
		UE_LOG(LogTemp, Display, TEXT("the number or unusedAssets in array object are: %d"), unusedAssetsArrayObjects.Num());
	}
	else
	{
		// Calling a UI notifications of there is not assets to clean in the content folder
		CallUnussedAssetsNotification();
	}
}

UFUNCTION()
void UCleanMasterWidget::CleanButtonPressed()
{
	if(unusedAssetsArray.Num() > 0)
	{
		UE_LOG(LogTemp, Display, TEXT("Calling Clean Button"));
		// first make a backup of the assets founded
		cleaner->BackupAssets();

		// show the process popup window
		CallCleanProcessPopup();
	}
	else
	{
		CallUnussedAssetsNotification();
	}
}

UFUNCTION()
void UCleanMasterWidget::RefreshButtonPressed()
{
	// TODO: Improve the refresh button functionality the idea is that the user can see whe he make a change in the name of any assets in the view list the update and when the cleaner button is pressed the view list empty without have to close the widget.
    UE_LOG(LogTemp, Display, TEXT("Calling Refresh button"));

	// Calling a notification of the assets are refreshed or update it.
    CallAssetsRefreshed();
}


UFUNCTION()
void UCleanMasterWidget::SettingsButtonPressed()
{
	UE_LOG(LogTemp, Display, TEXT("Calling Settings button"));
	
	// Crete the widget notification 
	UUserWidget* NotificationWidget = CreateWidget<UUserWidget>(GetWorld(), BP_SectionOnDevelopment);

	if (NotificationWidget)
	{
		// Add to the canvas main widget
		UPanelSlot* PanelSlot = RootCanvas->AddChild(NotificationWidget);
		
		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(PanelSlot))
		{
			CanvasSlot->SetPosition(FVector2D(380.f, 360.f)); // fixed position
		}
		
		TWeakObjectPtr<UUserWidget> WeakNotificationWidget = NotificationWidget;

		// To dessapear the message after 1 sec
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [WeakNotificationWidget]()
		{
			if (WeakNotificationWidget.IsValid())
			{
				WeakNotificationWidget->RemoveFromParent();
			}
		}, 1.0f, false);
	}
}

UFUNCTION()
void UCleanMasterWidget::PackageButtonPressed()
{
	UE_LOG(LogTemp, Display, TEXT("Calling Package button"));
	OpenPackagingSettingsWindow();
}

UFUNCTION()
void UCleanMasterWidget::PopulateListView()
{
	UE_LOG(LogTemp, Display, TEXT("Calling PopulateListView"));

	// Convert data to UObject be visualize in the list view
	ConvertUassetsToUobjects();
}

UFUNCTION()
void UCleanMasterWidget::ConvertUassetsToUobjects()
{
	for (const FAssetData& UnusedAsset : unusedAssetsArray)
	{
		UUAssetsData* NewAssetData = NewObject<UUAssetsData>(this);
		NewAssetData->AssetName = UnusedAsset.AssetName;
		NewAssetData->AssetPath = UnusedAsset.ObjectPath.ToString();
		NewAssetData->AssetType = UnusedAsset.GetClass()->GetName();
		NewAssetData->AssetSize = GetAssetSize(UnusedAsset);
		NewAssetData->AssetLastModified = GetAssetLastModified(UnusedAsset);
		unusedAssetsArrayObjects.AddUnique(NewAssetData);
	}
}

UFUNCTION()
void UCleanMasterWidget::CallBackupSuccessNotification()
{
	// Crete the widget notification 
	UUserWidget* NotificationWidget = CreateWidget<UUserWidget>(GetWorld(), BP_BackupSuccessNotification);

	if (NotificationWidget)
	{
		// Add to the canvas main widget
		UPanelSlot* PanelSlot = RootCanvas->AddChild(NotificationWidget);
		
		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(PanelSlot))
		{
			CanvasSlot->SetPosition(FVector2D(415.f, 420.f)); // fixed position
		}
		
		TWeakObjectPtr<UUserWidget> WeakNotificationWidget = NotificationWidget;

		// To dessapear the message after 1 sec
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [WeakNotificationWidget]()
		{
			if (WeakNotificationWidget.IsValid())
			{
				WeakNotificationWidget->RemoveFromParent();
			}
		}, 1.0f, false);
	}
}

UFUNCTION()
void UCleanMasterWidget::CallCleanProcessPopup()
{
	// Create the widget notification
	PopupWidget = CreateWidget<UUserWidget>(GetWorld(), BP_CleanProcessPopup);

    if (PopupWidget)
    {
	    // Adding the widget to the main canvas
    	UPanelSlot* PanelSlot = RootCanvas->AddChild(PopupWidget);
    	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(PanelSlot))
    	{
    		CanvasSlot->SetPosition(FVector2D(400.f, 250.f));
    	}
    	// get the image which contain the widget (GIF)
    	UImage* MediaImage = Cast<UImage>(PopupWidget->GetWidgetFromName(TEXT("GIF")));

    	if (MediaImage)
    	{
    		const FSlateBrush& Brush = MediaImage->GetBrush();
    		if (UMediaTexture* MediaTexture = Cast<UMediaTexture>(Brush.GetResourceObject()))
    		{
    			// get the media player associated with the Image
    			UMediaPlayer* MediaPlayer = MediaTexture->GetMediaPlayer();
    			UMediaSource* MediaSource = LoadObject<UMediaSource>(nullptr, TEXT("/Script/MediaAssets.FileMediaSource'/CleanMaster/Videos/RobotWave.RobotWave'"));
    			MediaPlayer->OpenSource(MediaSource);
    			MediaPlayer->Play();
    		}
    	}

    	// show the notification of success
    	CallBackupSuccessNotification();

    	// I want to execute the cleaning when the backup success notification happened
    	FTimerHandle TimerHandle;
    	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			if (cleaner->DeleteUnusedAsset( unusedAssetsArray) == 0)
			{
				CallCleaningSuccessNotification();
				CloseCleanProcessPopup();
			}
			else
			{
				CallCleaningFailNotification();
				CloseCleanProcessPopup();
			}
		}, 1.0f, false);
    }
}

UFUNCTION()
void UCleanMasterWidget::CallCleaningSuccessNotification()
{
	// Crete the widget notification 
	SuccessNotification = CreateWidget<UUserWidget>(GetWorld(), BP_CleaningSuccessNotification);

	if (SuccessNotification)
	{
		// Add to the canvas main widget
		UPanelSlot* PanelSlot = RootCanvas->AddChild(SuccessNotification);
		
		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(PanelSlot))
		{
			CanvasSlot->SetPosition(FVector2D(400.f, 360.f)); // fixed position
		}

		TWeakObjectPtr<UUserWidget> WeakNotificationWidget = SuccessNotification;

		// To dessapear the message after 1 sec
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [WeakNotificationWidget]()
		{
			if (WeakNotificationWidget.IsValid())
			{
				WeakNotificationWidget->RemoveFromParent();
			}
		}, 1.0f, false);
	}
}

UFUNCTION()
void UCleanMasterWidget::CallCleaningFailNotification()
{
	// Crete the widget notification 
	SuccessNotification = CreateWidget<UUserWidget>(GetWorld(),  BP_CleaningFailNotification);

	if (SuccessNotification)
	{
		// Add to the canvas main widget
		UPanelSlot* PanelSlot = RootCanvas->AddChild(SuccessNotification);
		
		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(PanelSlot))
		{
			CanvasSlot->SetPosition(FVector2D(400.f, 360.f)); // fixed position
		}

		TWeakObjectPtr<UUserWidget> WeakNotificationWidget = SuccessNotification;

		// To dessapear the message after 1 sec
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [WeakNotificationWidget]()
		{
			if (WeakNotificationWidget.IsValid())
			{
				WeakNotificationWidget->RemoveFromParent();
			}
		}, 1.0f, false);
	}
}

UFUNCTION()
void UCleanMasterWidget::CallAssetsRefreshed()
{
	// Crete the widget notification 
	SuccessNotification = CreateWidget<UUserWidget>(GetWorld(), BP_AssetsRefreshedNotification);

	if (SuccessNotification)
	{
		// Add to the canvas main widget
		UPanelSlot* PanelSlot = RootCanvas->AddChild(SuccessNotification);
		
		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(PanelSlot))
		{
			CanvasSlot->SetPosition(FVector2D(400.f, 360.f)); // fixed position
		}

		TWeakObjectPtr<UUserWidget> WeakNotificationWidget = SuccessNotification;

		// To dessapear the message after 1 sec
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [WeakNotificationWidget]()
		{
			if (WeakNotificationWidget.IsValid())
			{
				WeakNotificationWidget->RemoveFromParent();
			}
		}, 1.0f, false);
	}
}

UFUNCTION()
void UCleanMasterWidget::CallUnussedAssetsNotification()
{
	// Crete the widget notification 
	UUserWidget* NotificationUnnusedWidget = CreateWidget<UUserWidget>(GetWorld(), BP_UnusedAssetsEmptyNotification);

	if (NotificationUnnusedWidget)
	{
		// Add to the canvas main widget
		UPanelSlot* PanelSlot = RootCanvas->AddChild(NotificationUnnusedWidget);
		
		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(PanelSlot))
		{
			CanvasSlot->SetPosition(FVector2D(400.f, 360.f));
		}
		TWeakObjectPtr<UUserWidget> WeakNotificationWidget = NotificationUnnusedWidget;

		// To dessapear the message after 1 sec
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [WeakNotificationWidget]()
		{
			if (WeakNotificationWidget.IsValid())
			{
				WeakNotificationWidget->RemoveFromParent();
			}
		}, 1.0f, false);
	}
}

UFUNCTION()
void UCleanMasterWidget::CloseCleanProcessPopup()
{
	if (PopupWidget)
	{
		PopupWidget->RemoveFromParent(); 
		PopupWidget = nullptr;          
	}
}

// COMPLETELY GENERATED WITH AI ASSISTANCE: ChatGPT
UFUNCTION()
float UCleanMasterWidget::GetAssetSize(FAssetData Data)
{
	FString PackageName = Data.PackageName.ToString();
	FString FilePath = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension()); 

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	int64 AssetSizeBytes = PlatformFile.FileSize(*FilePath);

	if (AssetSizeBytes == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get size for asset at path: %s"), *FilePath);
		return 0.0f;
	}

	float AssetSizeKB = static_cast<float>(AssetSizeBytes) / 1024.0f;
	UE_LOG(LogTemp, Display, TEXT("Asset: %s | Size: %.2f KB"), *Data.AssetName.ToString(), AssetSizeKB);

	return AssetSizeKB;
}

// COMPLETELY GENERATED WITH AI ASSISTANCE: ChatGPT
UFUNCTION()
FDateTime UCleanMasterWidget::GetAssetLastModified(FAssetData Data)
{
	FString PackageName = Data.PackageName.ToString(); // Ej: /Game/MyFolder/MyAsset
	FString FilePath = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension()); // .uasset

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (PlatformFile.FileExists(*FilePath))
	{
		FFileStatData FileStats = PlatformFile.GetStatData(*FilePath);

		if (FileStats.bIsValid)
		{
			FDateTime LastModified = FileStats.ModificationTime;
			UE_LOG(LogTemp, Display, TEXT("Asset: %s | Last Modified: %s"), *Data.AssetName.ToString(), *LastModified.ToString());
			return LastModified;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not retrieve valid file stats for: %s"), *FilePath);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Asset file does not exist on disk: %s"), *FilePath);
	}

	return FDateTime(); 
}

UFUNCTION()
void UCleanMasterWidget::OpenPackagingSettingsWindow()
{
	ISettingsModule* SettingsModule =  FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	{
		SettingsModule->ShowViewer("Project", "Project", "Packaging");
	}
}