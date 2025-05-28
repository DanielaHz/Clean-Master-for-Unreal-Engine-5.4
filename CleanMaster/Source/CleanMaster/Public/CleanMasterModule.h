// Copyright Epic Games, Inc. All Rights Reserved.
// The following Unreal Engine class is based on the project cited below. I generated the modules from scratch, but the content follows the base structure of the plugin template created by Damian Nowakowski.
// Modified from:
// Damian Nowakowski (June 4, 2024). UE4EditorPluginTemplate [online].
// [Accessed 2016]. Available from: <https://github.com/zompi2/UE4EditorPluginTemplate>

#pragma once

#include "CoreMinimal.h"
#include "UObject/GCObject.h"
#include "Modules/ModuleManager.h"

/**
 *  Editor module which handles editor object and dockTab creation
 */

class CLEANMASTER_API FCleanMasterModule : public IModuleInterface, public FGCObject
{
public:
	// IModuleInterface implementation
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	// FGCObject implementation
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
#if (ENGINE_MAJOR_VERSION == 5)
	FString GetReferencerName() const override;
#endif

protected:
	void OnPostEngineInit(); // Run some intialization after the enfine has been initialized
private:
	bool CanSpawnEditor(); // Return true if the editor can be spawned
	TSharedRef<class SDockTab> SpawnEditor(const FSpawnTabArgs& Args);
	// Spawn editor and returns a ref of the docktab to which the ediotor
	bool IsEditorSpawned(); // checks if the editor is spawned
	void InvokeEditorSpawn(); // Invoke spawing editor from the command

#if (ENGINE_MAJOR_VERSION == 5)
	TObjectPtr<class UCleanMasterBase> Editor;
#else
	class UCleanMasterBase* Editor;
#endif

	//DockTab reference with the editor
	TWeakPtr<class SDockTab> EditorTab;

	// Handle for an OnPostEngineInit delegate
	FDelegateHandle OnPostEngineInitDelegateHandle;
};
