using UnrealBuildTool;

public class UnrealExamplesEditor : ModuleRules
{
    public UnrealExamplesEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                
				"Core",
			"CoreUObject",
			"Engine",
			"UnrealEd",
			"UnrealExamples"
			}
        );

    }
} 