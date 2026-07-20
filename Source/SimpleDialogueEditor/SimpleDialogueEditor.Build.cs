// Copyright (c) 2026 IchiQ. Licensed under the MIT License. See LICENSE for details.

using UnrealBuildTool;

public class SimpleDialogueEditor : ModuleRules
{
	public SimpleDialogueEditor(ReadOnlyTargetRules Target) : base(Target)
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
			// Our runtime data lives here
			"SimpleDialogueRuntime",

			// Editor / tooling
			"UnrealEd",
			"Projects",
			"ToolMenus",
			"AssetTools",
			"AssetDefinition",

			// Slate UI
			"Slate",
			"SlateCore",
			"InputCore",
			"EditorStyle",

			// Graph editing
			"PropertyEditor",
			"GraphEditor",
			"Kismet",
			"KismetWidgets",
			"ApplicationCore",
		});
	}
}
