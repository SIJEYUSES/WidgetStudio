// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

/**
 * Editor module .build.cs
 */
public class WidgetStudioEditor : ModuleRules
{
	public WidgetStudioEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);


		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);


		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"UMG",
				"Blutility",
				"UnrealEd",
				"UMGEditor",
				"WidgetStudioRuntime",
				"PropertyEditor",
				"EditorStyle",
				"DetailCustomizations",
				"EditorScriptingUtilities"
				// ... add other public dependencies that you statically link with here ...
			}
			);

#if UE_5_0_OR_LATER
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"DeveloperToolSettings"
			}
		);
#endif

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...
			}
			);

		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

	}
}
