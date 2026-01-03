// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

[SupportedPlatforms(UnrealPlatformClass.Server)]
public class LochStarterServerTarget : TargetRules
{
	public LochStarterServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;
		ExtraModuleNames.AddRange(new string[] { "LochStarterGame" });
		LochStarterGameTarget.ApplySharedLochStarterTargetSettings(this);
		bUseChecksInShipping = true;
	}
}
