// Copyright Epic Games, Inc. All Rights Reserved.
// The following Unreal Engine class is based on the project cited below. I generated the modules from scratch, but the content follows the base structure of the plugin template created by Damian Nowakowski.
// Modified from:
// Damian Nowakowski (June 4, 2024). UE4EditorPluginTemplate [online].
// [Accessed 2016]. Available from: <https://github.com/zompi2/UE4EditorPluginTemplate>

#pragma once

#include "CoreMinimal.h"
#include "Shader.h"
#include "UObject/Object.h"
#include "CleanMasterBase.generated.h"

/**
 * Object handles all of the basic logic of the plugin.
 */
UCLASS()
class CLEANMASTER_API UCleanMasterBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void Init(); // Initializes the editor object. Runs right after it's creation.
	void SetEditorTab(const TSharedRef<class SDockTab>& NewEditorTab);
	void CreateUtilityWidgetBlueprint();
	// Sets up the EditorTab. Used by editor MOdule right after a tan is created.
	bool CanCreateEditorUI();;
	//Creates Editor UI widget and returns a reference to it. Its called form the editor module in a moment of Tab Creation
	TSharedRef<class SWidget> CreateEditorUI();

protected:
	virtual void InitializeTheWidget();
	UPROPERTY()
	class UCleanMasterWidget* EditorWidget;

private:
	static class UEditorUtilityWidgetBlueprint* GetUtilityWidgetBlueprint();
	// Returns an Utility Widget Blueprint form conten directory which will be used to create an editor options window
	TSharedRef<SWidget> CreateEditorWidget();
	// Creates the Editor utility widget form the utility widget blueprint
	void ChangeTabWorld(UWorld* World, EMapChangeType MapChangeType);
	// Called when OnMapChanged event occurs. The EditorTab must be properly
	TWeakPtr<class SDockTab> EditorTab; // A pointer to the EditorTab in which the editor widget should be docked.
	// FDelegateHandle OnPostEngineInitDelegateHandle;
};
