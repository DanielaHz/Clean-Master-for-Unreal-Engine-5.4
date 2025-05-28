// Copyright Epic Games, Inc. All Rights Reserved.

#include "CleanMasterCommands.h"
// #include "EditorStyleSet.h"


FCleanMasterCommands::FCleanMasterCommands() :
	TCommands<FCleanMasterCommands>(
		TEXT("Clean master Commands"),
		FText::FromString(TEXT("Commands to control the Plugin")),
		NAME_None,
#if ((ENGINE_MAJOR_VERSION == 5) && (ENGINE_MINOR_VERSION >= 1))
		FAppStyle::GetAppStyleSetName()
#else
		FEditorStyle::GetStyleSetName()
#endif
	)
{
}

#define LOCTEXT_NAMESPACE "FCleanMasterModule"

void FCleanMasterCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "CleanMaster", "Bring up CleanMaster window", EUserInterfaceActionType::Check,
	           FInputChord(EModifierKey::Shift | EModifierKey::Alt, EKeys::C));
}

#undef LOCTEXT_NAMESPACE
