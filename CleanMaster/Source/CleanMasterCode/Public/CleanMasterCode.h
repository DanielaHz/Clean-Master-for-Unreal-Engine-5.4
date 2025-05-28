#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include <filesystem>
#include <iostream>
#include "ContentBrowserModule.h" // FContentBrowserModule API to interact with the content browser. 

/*
 * This class is created by default when the module is created. Even if it is empty, it is important to keep it because it serves as the entry point of the module.
 */

class FCleanMasterCodeModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
