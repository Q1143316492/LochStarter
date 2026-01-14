// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettingsBackedByCVars.h"
#include "UObject/SoftObjectPath.h"

#include "CommonLoadingScreenSettings.generated.h"

class UObject;

/**
 * Settings for a loading screen system.
 * 加载界面系统的设置。
 */
UCLASS(config=Game, defaultconfig, meta=(DisplayName="Common Loading Screen"))
class UCommonLoadingScreenSettings : public UDeveloperSettingsBackedByCVars
{
	GENERATED_BODY()

public:
	UCommonLoadingScreenSettings();

public:
	
	// The widget to load for the loading screen.
	// 加载界面要加载的控件。
	UPROPERTY(config, EditAnywhere, Category=Display, meta=(MetaClass="/Script/UMG.UserWidget"))
	FSoftClassPath LoadingScreenWidget;

	// The z-order of the loading screen widget in the viewport stack
	// 加载界面控件在视口堆栈中的Z序
	UPROPERTY(config, EditAnywhere, Category=Display)
	int32 LoadingScreenZOrder = 10000;

	// How long to hold the loading screen up after other loading finishes (in seconds) to
	// try to give texture streaming a chance to avoid blurriness
	//
	// Note: This is not normally applied in the editor for iteration time, but can be 
	// enabled via HoldLoadingScreenAdditionalSecsEvenInEditor
	// 在其他加载完成后，加载界面继续保持显示的时间（秒），以便给纹理流提供机会，避免模糊。
	// 注意：通常在编辑器中不会应用此设置以加快迭代速度，但可以通过 HoldLoadingScreenAdditionalSecsEvenInEditor 启用。
 	UPROPERTY(config, EditAnywhere, Category=Configuration, meta=(ForceUnits=s, ConsoleVariable="CommonLoadingScreen.HoldLoadingScreenAdditionalSecs"))
	float HoldLoadingScreenAdditionalSecs = 2.0f;

	// The interval in seconds beyond which the loading screen is considered permanently hung (if non-zero).
	// 超过该秒数后，若加载界面仍未关闭，则认为加载界面已永久卡死（非零时生效）。
 	UPROPERTY(config, EditAnywhere, Category=Configuration, meta=(ForceUnits=s))
	float LoadingScreenHeartbeatHangDuration = 0.0f;

	// The interval in seconds between each log of what is keeping a loading screen up (if non-zero).
	// 每隔多少秒记录一次导致加载界面持续显示的原因（非零时生效）。
 	UPROPERTY(config, EditAnywhere, Category=Configuration, meta=(ForceUnits=s))
	float LogLoadingScreenHeartbeatInterval = 5.0f;

	// When true, the reason the loading screen is shown or hidden will be printed to the log every frame.
	// 若为true，每帧都在日志中打印加载界面显示或隐藏的原因。
	UPROPERTY(Transient, EditAnywhere, Category=Debugging, meta=(ConsoleVariable="CommonLoadingScreen.LogLoadingScreenReasonEveryFrame"))
	bool LogLoadingScreenReasonEveryFrame = 0;

	// Force the loading screen to be displayed (useful for debugging)
	// 强制显示加载界面（用于调试）
	UPROPERTY(Transient, EditAnywhere, Category=Debugging, meta=(ConsoleVariable="CommonLoadingScreen.AlwaysShow"))
	bool ForceLoadingScreenVisible = false;

	// Should we apply the additional HoldLoadingScreenAdditionalSecs delay even in the editor
	// (useful when iterating on loading screens)
	// 是否在编辑器中也应用 HoldLoadingScreenAdditionalSecs 的额外延迟（便于迭代加载界面时使用）
	UPROPERTY(Transient, EditAnywhere, Category=Debugging)
	bool HoldLoadingScreenAdditionalSecsEvenInEditor = false;

	// Should we apply the additional HoldLoadingScreenAdditionalSecs delay even in the editor
	// (useful when iterating on loading screens)
	// 是否在编辑器中也应用 HoldLoadingScreenAdditionalSecs 的额外延迟（便于迭代加载界面时使用）
	UPROPERTY(config, EditAnywhere, Category=Configuration)
	bool ForceTickLoadingScreenEvenInEditor = true;
};

