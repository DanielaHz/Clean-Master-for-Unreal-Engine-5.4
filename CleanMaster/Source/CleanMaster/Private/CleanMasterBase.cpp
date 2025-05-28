// Copyright Epic Games, Inc. All Rights Reserved.

#include "CleanMasterBase.h"
#include "CleanMasterWidget.h"

// include the widget here
#include "EditorUtilityWidget.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "LevelEditor.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/ConstructorHelpers.h"

void UCleanMasterBase::Init()
{
}

void UCleanMasterBase::InitializeTheWidget()
{
}

void UCleanMasterBase::SetEditorTab(const TSharedRef<class SDockTab>& NewEditorTab)
{
	EditorTab = NewEditorTab;
}

UEditorUtilityWidgetBlueprint* UCleanMasterBase::GetUtilityWidgetBlueprint()
{
	// Get the editor utility widget blueprint from the content directory
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<
		FAssetRegistryModule>("AssetRegistry");
#if (ENGINE_MAJOR_VERSION == 5) && (ENGINE_MINOR_VERSION >= 1)
	FAssetData AssetData = AssetRegistryModule.Get().GetAssetByObjectPath(FSoftObjectPath("/CleanMaster/CleanMasterWidget.CleanMasterWidget"));
#else
	FAssetData AssetData = AssetRegistryModule.Get().GetAssetByObjectPath(
		"/CleanMaster/CleanMasterWidget.CleanMasterWidget");
#endif
	return Cast<UEditorUtilityWidgetBlueprint>(AssetData.GetAsset());
}


bool UCleanMasterBase::CanCreateEditorUI()
{
	return GetUtilityWidgetBlueprint() != nullptr;
}

TSharedRef<class SWidget> UCleanMasterBase::CreateEditorUI()
{
	// Register OnMapChanged event so we can properly handle Tab and Widget when changing levels.
	FLevelEditorModule& LevelEditor = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	LevelEditor.OnMapChanged().AddUObject(this, &UCleanMasterBase::ChangeTabWorld);

	// Create the Widget
	return CreateEditorWidget();
}

TSharedRef<SWidget> UCleanMasterBase::CreateEditorWidget()
{
	TSharedRef<SWidget> CreatedWidget = SNullWidget::NullWidget;
	if (UEditorUtilityWidgetBlueprint* UtilityWidgetBP = GetUtilityWidgetBlueprint())
	{
		CreatedWidget = UtilityWidgetBP->CreateUtilityWidget();
		EditorWidget = Cast<UCleanMasterWidget>(UtilityWidgetBP->GetCreatedWidget());
		if (EditorWidget)
		{
			InitializeTheWidget();
		}
	}
	return CreatedWidget;
}

void UCleanMasterBase::ChangeTabWorld(UWorld* World, EMapChangeType MapChangeType)
{
	// Handle the event when editor map changes.
	if (MapChangeType == EMapChangeType::TearDownWorld)
	{
		// If the world is destroyed - set the Tab content to null and null the Widget.
		if (EditorTab.IsValid())
		{
			EditorTab.Pin()->SetContent(SNullWidget::NullWidget);
		}
		if (EditorWidget)
		{
			EditorWidget->Rename(nullptr, GetTransientPackage());
			EditorWidget = nullptr;
		}
	}
	else if (MapChangeType == EMapChangeType::NewMap || MapChangeType == EMapChangeType::LoadMap)
	{
		// If the map has been created or loaded and the Tab is valid - put a new Widget into this Tab.
		if (EditorTab.IsValid())
		{
			EditorTab.Pin()->SetContent(CreateEditorWidget());
		}
	}
}
