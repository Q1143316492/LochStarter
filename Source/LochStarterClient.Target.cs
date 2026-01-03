// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class LochStarterClientTarget : TargetRules
{
	public LochStarterClientTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Client;
		ExtraModuleNames.Add("LochStarterGame");
		LochStarterGameTarget.ApplySharedLochStarterTargetSettings(this);
	}
}
