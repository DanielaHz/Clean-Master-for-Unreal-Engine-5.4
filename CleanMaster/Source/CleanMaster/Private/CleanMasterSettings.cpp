// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CleanMasterSettings.h"
#include "Runtime/Launch/Resources/Version.h"

bool UCleanMasterSettings::GetIsCheckboxChecked()
{
	return GetDefault<UCleanMasterSettings>()->bIsCheckboxChecked;
}

void UCleanMasterSettings::SetIsCheckboxChecked(bool bInIsCheckboxChecked)
{
	UCleanMasterSettings* Settings = GetMutableDefault<UCleanMasterSettings>();
	Settings->bIsCheckboxChecked = bInIsCheckboxChecked;

#if (ENGINE_MAJOR_VERSION >= 5)
	Settings->TryUpdateDefaultConfigFile();
#else
	Settings -
#.UpdateDefaultConfigFile();
#endif
}
