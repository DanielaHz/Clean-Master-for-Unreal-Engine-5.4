// Copyright Epic Games, Inc. All Rights Reserved.

#include "CleanMaster.h"
#include "CleanMasterWidget.h"
#include "CleanMasterSettings.h"
#include "EditorUtilityWidget.h"
#include "EditorUtilitySubsystem.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "LevelEditor.h"

void UCleanMaster::Init()
{
}

void UCleanMaster::InitializeTheWidget()
{
	// Initialize the widget here
	EditorWidget->SetDesiredSizeInViewport(FVector2D(1280, 720)); 
}
