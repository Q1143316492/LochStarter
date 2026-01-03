// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class LochStarterEditorTarget : TargetRules
{
	public LochStarterEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		BuildEnvironment = TargetBuildEnvironment.Unique;
		ExtraModuleNames.AddRange(new string[] { "LochStarterGame", "LochStarterEditor" });

		if (!bBuildAllModules)
		{
			// 不允许用原生指针用于控制UCLASS, USTRUCT。要用TObjectPtr, 否则UHT就报错
			NativePointerMemberBehaviorOverride = PointerMemberBehavior.Disallow;
		}

		LochStarterGameTarget.ApplySharedLochStarterTargetSettings(this);

		// This is used for touch screen development along with the "Unreal Remote 2" app
		EnablePlugins.Add("RemoteSession");
	}
}
