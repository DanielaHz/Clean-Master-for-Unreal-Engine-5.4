// Copyright Epic Games, Inc. All Rights Reserved.
// The following Unreal Engine class is based on the project cited below. I generated the modules from scratch, but the content follows the base structure of the plugin template created by Damian Nowakowski.
// Modified from:
// Damian Nowakowski (June 4, 2024). UE4EditorPluginTemplate [online].
// [Accessed 2016]. Available from: <https://github.com/zompi2/UE4EditorPluginTemplate>

#pragma once

#include "CleanMasterBase.h"
#include "CleanMaster.generated.h"

UCLASS()
class CLEANMASTER_API UCleanMaster : public UCleanMasterBase
{
	GENERATED_BODY()

public:
	virtual void Init() override; // UCleanMasterBase
	
protected:
	virtual void InitializeTheWidget() override;
};
