// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FanMeetingDemo : ModuleRules
{
	public FanMeetingDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "UniversalVoiceChatPro", "Voice" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// "Engine/"
		string EnginePath = System.IO.Path.GetFullPath(Target.RelativeEnginePath);
		string PlugInPath = EnginePath + "Plugins/Marketplace/UniversalVoiceChatPro/Source/UniversalVoiceChatPro/Private";

		PublicIncludePaths.AddRange(new string[]
		{
			PlugInPath
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
