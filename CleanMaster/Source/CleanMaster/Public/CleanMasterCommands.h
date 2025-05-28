// Copyright Epic Games, Inc. All Rights Reserved.
// The following Unreal Engine class is based on the project cited below. I generated the modules from scratch, but the content follows the base structure of the plugin template created by Damian Nowakowski.
// Modified from:
// Damian Nowakowski (June 4, 2024). UE4EditorPluginTemplate [online].
// [Accessed 2016]. Available from: <https://github.com/zompi2/UE4EditorPluginTemplate>

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"

class CLEANMASTER_API FCleanMasterCommands : public TCommands<FCleanMasterCommands>
{
public:
	FCleanMasterCommands();

	// TCommands<> interface
	virtual void RegisterCommands() override;

	TSharedPtr<FUICommandInfo> OpenPluginWindow;
};
