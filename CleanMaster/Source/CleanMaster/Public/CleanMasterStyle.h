// Copyright Epic Games, Inc. All Rights Reserved.
// The following Unreal Engine class is based on the project cited below. I generated the modules from scratch, but the content follows the base structure of the plugin template created by Damian Nowakowski.
// Modified from:
// Damian Nowakowski (June 4, 2024). UE4EditorPluginTemplate [online].
// [Accessed 2016]. Available from: <https://github.com/zompi2/UE4EditorPluginTemplate>

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

/**
 *  Handles the styling of the editor. Currently used only fo menu and tab icon
 */
class CLEANMASTER_API FCleanMasterStyle
{
public:
	static void Initialize();

	static void Shutdown();

	/** reloads textures used by slate renderer */
	static void ReloadTextures();

	/** @return The Slate style set for the Shooter game */
	static const ISlateStyle& Get();

	static FName GetStyleSetName();

private:
	static TSharedRef<class FSlateStyleSet> Create();

	static TSharedPtr<class FSlateStyleSet> StyleInstance;
};
