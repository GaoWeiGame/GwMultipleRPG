// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GwMultipleRPG : ModuleRules
{
	public GwMultipleRPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameFeatures", "GameplayTags",
			"AIModule",

			"GameplayTasks"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Slate",
			"SlateCore", "GameplayTags", "GameplayTasks", "GameplayAbilities", "ModularGameplay", 
			"UMG",
			"CommonGame", "CommonUI", "CommonInput", "CommonUser",
			"GameSettings",
			"ModularGameplayActors", "NetCore", "GameplayMessageRuntime", "UIExtension", "Niagara"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
		SetupIrisSupport(Target);
	}
}