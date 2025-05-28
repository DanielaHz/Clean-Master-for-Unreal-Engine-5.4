// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CleanMaster : ModuleRules
{
	public CleanMaster(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PrivateIncludePaths.Add("CleanMaster/Private");
		PrivateIncludePaths.Add("CleanMasterCode/Private");

		// Dependencias públicas que siempre se requieren
		PublicDependencyModuleNames.AddRange(
			new[] 
			{
				"Core",
				"InputCore",
				"AssetRegistry",
				"CleanMasterCode"
			}
		);

		// Dependencias privadas
		PrivateDependencyModuleNames.AddRange(
			new[] 
			{
				"Projects",
				"InputCore",
				"EditorFramework",
				"ToolMenus",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore", 
				"Blutility", 
				"UMG", 
				"UMGEditor", 
				"WorkspaceMenuStructure",
				"DesktopPlatform", 
				"EditorInteractiveToolsFramework", 
				"MediaAssets", 
				"LevelEditor"
			}
		);

		// Agregar UnrealEd solo para el target Editor
		if (Target.Type == TargetType.Editor)
		{
			PrivateDependencyModuleNames.Add("UnrealEd");
		}

		// No cargar módulos dinámicamente en este caso
		DynamicallyLoadedModuleNames.AddRange(new string[] {});
	}
}