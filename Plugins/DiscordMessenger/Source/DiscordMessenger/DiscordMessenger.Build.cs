// Copyright © 2020 David ten Kate - All Rights Reserved.

using UnrealBuildTool;

public class DiscordMessenger : ModuleRules
{
	public DiscordMessenger(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core" });

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"Http",
				"Json",
				"JsonUtilities"
			}
			);
	}
}
