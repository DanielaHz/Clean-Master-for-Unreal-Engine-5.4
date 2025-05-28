// Copyright Epic Games, Inc. All Rights Reserved.

#include "CleanMasterStyle.h"

#include "../../../../../../../../../Program Files/Epic Games/UE_5.4/Engine/Source/Runtime/SlateCore/Public/Styling/SlateStyleMacros.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FCleanMasterStyle::StyleInstance = nullptr;

void FCleanMasterStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FCleanMasterStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FCleanMasterStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("CleanMasterStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef<FSlateStyleSet> FCleanMasterStyle::Create()
{
	TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet("CleanMasterStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("CleanMaster")->GetBaseDir() / TEXT("Resources"));

	Style->Set("CleanMaster.OpenPluginWindow", new IMAGE_BRUSH(TEXT("Icon128.png"), Icon20x20));

	return Style;
}

void FCleanMasterStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FCleanMasterStyle::Get()
{
	return *StyleInstance;
}
