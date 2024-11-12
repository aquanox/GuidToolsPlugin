// Copyright 2024, Aquanox.

using UnrealBuildTool;

public class GuidToolsCore : ModuleRules
{
	private bool bStrictIncludesCheck = false;

	public GuidToolsCore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// Disable private/public structure
		PublicIncludePaths.Add(ModuleDirectory);

		// This is to emulate engine installation and verify includes during development
		// Gives effect similar to BuildPlugin with -StrictIncludes
		if (bStrictIncludesCheck)
		{
			bUseUnity = false;
			PCHUsage = PCHUsageMode.NoPCHs;
			bTreatAsEngineModule = true;
		}

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
		});
	}
}
