using UnrealBuildTool;

public class CleanMasterCode : ModuleRules
{
	public CleanMasterCode(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PrivateIncludePaths.Add("CleanMasterCode/Private");

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"InputCore"
			}
		);
		
		if (Target.Type == TargetType.Editor)
		{
			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"ContentBrowser",
					"AssetTools",    
					"Blutility",    
					"UnrealEd"
				}
			);
		}
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"InputCore",
				"AutomationController",
				"AutomationUtils"
			}
		);
	}
}