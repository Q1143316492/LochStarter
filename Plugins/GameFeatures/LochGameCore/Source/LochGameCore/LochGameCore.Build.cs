using UnrealBuildTool;

public class LochGameCore : ModuleRules
{
    public LochGameCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
				"Core",
				// "LochStarterGame",
				"ModularGameplay",
				"CommonGame",
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"GameplayTags",
				"GameplayTasks",
				"GameplayAbilities",
				"GameplayMessageRuntime",
				"CommonUI",
				"UMG",
				"DataRegistry",
				"AsyncMixin",
				"EnhancedInput",
				"GameSubtitles",
				"DeveloperSettings",
				"AIModule"
        });
    }
}
