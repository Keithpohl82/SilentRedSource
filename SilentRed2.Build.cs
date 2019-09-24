// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SilentRed2 : ModuleRules
{
    

	public SilentRed2(ReadOnlyTargetRules Target) : base(Target)
	{
        

		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG" , "OnlineSubsystem", "OnlineSubsystemSteam" });
	}
}
