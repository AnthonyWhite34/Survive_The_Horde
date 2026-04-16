// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Survive_The_Horde : ModuleRules
{
	public Survive_The_Horde(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"GameplayAbilities",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"GameplayTags",
			"GameplayTasks"
		});

		PublicIncludePaths.AddRange(new string[] {
			"Survive_The_Horde",
			"Survive_The_Horde/Variant_Platforming",
			"Survive_The_Horde/Variant_Platforming/Animation",
			"Survive_The_Horde/Variant_Combat",
			"Survive_The_Horde/Variant_Combat/AI",
			"Survive_The_Horde/Variant_Combat/Animation",
			"Survive_The_Horde/Variant_Combat/Gameplay",
			"Survive_The_Horde/Variant_Combat/Interfaces",
			"Survive_The_Horde/Variant_Combat/UI",
			"Survive_The_Horde/Variant_SideScrolling",
			"Survive_The_Horde/Variant_SideScrolling/AI",
			"Survive_The_Horde/Variant_SideScrolling/Gameplay",
			"Survive_The_Horde/Variant_SideScrolling/Interfaces",
			"Survive_The_Horde/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
