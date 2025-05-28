// Fill out your copyright notice in the Description page of Project Settings.


#include "CleanMasterModule.h"
#include "CleanMaster.h"
#include "CleanMasterCommands.h"
#include "CleanMasterStyle.h"
#include "Widgets/Docking/SDockTab.h"
#include "Framework/Docking/TabManager.h"
#include "Interfaces/IMainFrameModule.h"

#include "PropertyEditorModule.h"
#include "LevelEditor.h"
#include "Editor/Experimental/EditorInteractiveToolsFramework/Public/Behaviors/2DViewportBehaviorTargets.h"

IMPLEMENT_MODULE(FCleanMasterModule, FCleanMaster)

const FName CleanMasterTabId = FName
(TEXT
	(
		"CleanMaster"
	)
);

void FCleanMasterModule::StartupModule()
{
	// Register styles
	FCleanMasterStyle::Initialize();
	FCleanMasterStyle::ReloadTextures();

	//Register UICommands
	FCleanMasterCommands::Register();

	// Register OnPostEngineInit delegate
	OnPostEngineInitDelegateHandle = FCoreDelegates::OnPostEngineInit.AddRaw(
		this, &FCleanMasterModule::OnPostEngineInit);

	// Create and initialize editor object
	Editor = NewObject<UCleanMasterBase>(GetTransientPackage(), UCleanMaster::StaticClass());
	Editor->Init();

	// Register tab spawner. Do not show it menu as it will be invoked manually by a UICommand
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		                        CleanMasterTabId,
		                        FOnSpawnTab::CreateRaw(this, &FCleanMasterModule::SpawnEditor),
		                        FCanSpawnTab::CreateLambda([this](const FSpawnTabArgs& Args) -> bool
		                        {
			                        return CanSpawnEditor();
		                        })
	                        )
	                        .SetMenuType(ETabSpawnerMenuType::Hidden)
	                        .SetIcon(
		                        FSlateIcon(FCleanMasterStyle::GetStyleSetName(), "CleanMasterEditorStyle.MenuIcon"));
}

void FCleanMasterModule::ShutdownModule()
{
	// Unregister Tab Spawner
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(CleanMasterTabId);

	// Cleanup the Editor object.
	Editor = nullptr;

	// Remove OnPostEngineInit delegate
	FCoreDelegates::OnPostEngineInit.Remove(OnPostEngineInitDelegateHandle);

	// Unregister UICommands.
	FCleanMasterCommands::Unregister();

	// Unregister Styles.
	FCleanMasterStyle::Shutdown();
}

void FCleanMasterModule::OnPostEngineInit()
{
	// This function is for registering UICommand to the engine, so it can be executed via keyboard shortcut.
	// This will also add this UICommand to the menu, so it can also be executed from there.

	// This function is valid only if no Commandlet or game is running. It also requires Slate Application to be initialized.
	if ((IsRunningCommandlet() == false) && (IsRunningGame() == false) && FSlateApplication::IsInitialized())
	{
		if (FLevelEditorModule* LevelEditor = FModuleManager::LoadModulePtr<FLevelEditorModule>(TEXT("LevelEditor")))
		{
			// Create a UICommandList and map editor spawning function to the UICommand of opening My Plugin Editor.
			TSharedPtr<FUICommandList> Commands = MakeShareable(new FUICommandList());
			Commands->MapAction(
				FCleanMasterCommands::Get().OpenPluginWindow,
				FExecuteAction::CreateRaw(this, &FCleanMasterModule::InvokeEditorSpawn),
				FCanExecuteAction::CreateRaw(this, &FCleanMasterModule::CanSpawnEditor),
				FIsActionChecked::CreateRaw(this, &FCleanMasterModule::IsEditorSpawned)
			);

			// Register this UICommandList to the MainFrame.
			// Otherwise nothing will handle the input to trigger this command.
			IMainFrameModule& MainFrame = FModuleManager::Get().LoadModuleChecked<IMainFrameModule>("MainFrame");
			MainFrame.GetMainFrameCommandBindings()->Append(Commands.ToSharedRef());

#if ((ENGINE_MAJOR_VERSION == 5) && (ENGINE_MINOR_VERSION >= 4))
			// The Menu Extender doesn't work correctly for new menus in UE5.4 as they don't have proper Hook names as they should...
			// Attempt to add menu entry with UICommandList of opening MyPlugin Window to the Tools menu.
			UToolMenu* Menu = UToolMenus::Get()->FindMenu("LevelEditor.MainMenu.Tools");
			if (Menu)
			{
				Menu->AddMenuEntry(NAME_None, FToolMenuEntry::InitMenuEntryWithCommandList(
					FCleanMasterCommands::Get().OpenPluginWindow,
					Commands,
					FText::FromString(TEXT("Clean Master")),
					FText::FromString(TEXT("Clean up projects to Unreal Engine 5")),
					FSlateIcon(FCleanMasterStyle::GetStyleSetName(), "MyPluginEditorStyle.MenuIcon")
				));
			}
#else
			// Create a Menu Extender, which adds a button that executes the UICommandList of opening My Plugin Window.
			TSharedPtr<FExtender> MainMenuExtender = MakeShareable(new FExtender);
			MainMenuExtender->AddMenuExtension(
#if (ENGINE_MAJOR_VERSION == 5)
				FName(TEXT("Tools")),
#else
				FName(TEXT("General")),
#endif
				EExtensionHook::After,
				Commands,
				FMenuExtensionDelegate::CreateLambda([](FMenuBuilder& MenuBuilder)
				{
					MenuBuilder.AddMenuEntry(
						FMyPluginEditorCommands::Get().OpenMyPluginWindow,
						NAME_None,
						FText::FromString(TEXT("My Plugin")),
						FText::FromString(TEXT("Opens My Plugin Window")),
						FSlateIcon(FMyPluginEditorStyle::GetStyleSetName(), "MyPluginEditorStyle.MenuIcon")
					);
				})
			);

			// Extend Editors menu with the created Menu Extender.
			LevelEditor->GetMenuExtensibilityManager()->AddExtender(MainMenuExtender);
#endif
		}
	}
}

void FCleanMasterModule::AddReferencedObjects(FReferenceCollector& Collector)
{
	// Prevent Editor Object from being garbage collected.
	if (Editor)
	{
		Collector.AddReferencedObject(Editor);
	}
}

#if (ENGINE_MAJOR_VERSION == 5)
FString FCleanMasterModule::GetReferencerName() const
{
	return TEXT("MyPluginModuleGCObject");
}
#endif

bool FCleanMasterModule::CanSpawnEditor()
{
	// Editor can be spawned only when the Editor object say that UI can be created.
	if (Editor && Editor
		->
		CanCreateEditorUI()
	)
	{
		return true;
	}
	return false;
}

TSharedRef<SDockTab> FCleanMasterModule::SpawnEditor(const FSpawnTabArgs& Args)
{
	// Spawn the Editor only when we can.
	if (CanSpawnEditor())
	{
		// Spawn new DockTab and fill it with newly created editor UI.
		TSharedRef<SDockTab> NewTab = SAssignNew(EditorTab, SDockTab).TabRole(NomadTab)
			[
				Editor->
				CreateEditorUI()
			];

		// Tell the Editor Object about newly spawned DockTab, as it will 
		// need it to handle various editor actions.
		Editor->SetEditorTab(NewTab);

		// Return the DockTab to the Global Tab Manager.
		return NewTab;
	}

	// If editor can't be spawned - create an empty tab.
	return SAssignNew(EditorTab, SDockTab).TabRole(NomadTab);
}

bool FCleanMasterModule::IsEditorSpawned()
{
	// Checks if the editor tab is already existing in the editor
	return FGlobalTabmanager::Get()->FindExistingLiveTab(CleanMasterTabId).IsValid();
}

void FCleanMasterModule::InvokeEditorSpawn()
{
	// Tries to invoke opening a plugin tab
	FGlobalTabmanager::Get()->TryInvokeTab(CleanMasterTabId);
}
