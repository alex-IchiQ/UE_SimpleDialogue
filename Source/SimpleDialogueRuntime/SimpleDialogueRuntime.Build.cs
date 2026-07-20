// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.

using UnrealBuildTool;

public class SimpleDialogueRuntime : ModuleRules
{
	public SimpleDialogueRuntime(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		IWYUSupport = IWYUSupport.Full;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			// Add gameplay-facing runtime deps here (GameplayTags, etc.)
		});
	}
}
