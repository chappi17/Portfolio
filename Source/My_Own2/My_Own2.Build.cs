// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class My_Own2 : ModuleRules
{
	public My_Own2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore","GameplayTasks", "AIModule" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		PublicIncludePaths.Add(ModuleDirectory);

	}
}
