// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.

using UnrealBuildTool;

// Optional sample module. It only demonstrates how to use the plugin and is safe to delete:
// remove this folder and the "SimpleDialogueSample" entry from SimpleDialogue.uplugin.
public class SimpleDialogueSample : ModuleRules
{
	public SimpleDialogueSample(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		IWYUSupport = IWYUSupport.Full;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",

			// The plugin these samples are built on top of.
			"SimpleDialogueRuntime",
		});
	}
}
