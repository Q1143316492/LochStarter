// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettingsBackedByCVars.h"
#include "Engine/PlatformSettings.h"
#include "GameplayTagContainer.h"
#include "Performance/LochPerformanceStatTypes.h"

#include "LochPerformanceSettings.generated.h"

class UObject;

// Describes one platform-specific device profile variant that the user can choose from in the UI
USTRUCT()
struct FLochQualityDeviceProfileVariant
{
	GENERATED_BODY()

	// The display name for this device profile variant (visible in the options screen)
	UPROPERTY(EditAnywhere)
	FText DisplayName;

	// The suffix to append to the base device profile name for the current platform
	UPROPERTY(EditAnywhere)
	FString DeviceProfileSuffix;

	// The minimum required refresh rate to enable this mode
	// (e.g., if this is set to 120 Hz and the device is connected
	// to a 60 Hz display, it won't be available)
	UPROPERTY(EditAnywhere)
	int32 MinRefreshRate = 0;
};

// Describes a set of performance stats that the user can enable in settings,
// predicated on passing a visibility query on platform traits
USTRUCT()
struct FLochPerformanceStatGroup
{
	GENERATED_BODY()

	// A query on platform traits to determine whether or not it will be possible
	// to show a set of stats
	UPROPERTY(EditAnywhere, meta=(Categories = "Input,Platform.Trait"))
	FGameplayTagQuery VisibilityQuery;

	// The set of stats to allow if the query passes
	UPROPERTY(EditAnywhere)
	TSet<ELochDisplayablePerformanceStat> AllowedStats;
};

// How hare frame pacing and overall graphics settings controlled/exposed for the platform?
UENUM()
enum class ELochFramePacingMode : uint8
{
	// Manual frame rate limits, user is allowed to choose whether or not to lock to vsync
	DesktopStyle,

	// Limits handled by choosing present intervals driven by device profiles
	ConsoleStyle,

	// Limits handled by a user-facing choice of frame rate from among ones allowed by device profiles for the specific device
	MobileStyle
};

UCLASS(config=Game, defaultconfig)
class ULochPlatformSpecificRenderingSettings : public UPlatformSettings
{
	GENERATED_BODY()

public:
	ULochPlatformSpecificRenderingSettings();

	// Helper method to get the performance settings object, directed via platform settings
	static const ULochPlatformSpecificRenderingSettings* Get();

public:
	// The default variant suffix to append, should typically be a member of
	// UserFacingDeviceProfileOptions unless there is only one for the current platform
	//
	// Note that this will usually be set from platform-specific ini files, not via the UI
	UPROPERTY(EditAnywhere, Config, Category=DeviceProfiles)
	FString DefaultDeviceProfileSuffix;

	// The list of device profile variations to allow users to choose from in settings
	//
	// These should be sorted from slowest to fastest by target frame rate:
	//   If the current display doesn't support a user chosen refresh rate, we'll try
	//   previous entries until we find one that works
	//
	// Note that this will usually be set from platform-specific ini files, not via the UI
	UPROPERTY(EditAnywhere, Config, Category=DeviceProfiles)
	TArray<FLochQualityDeviceProfileVariant> UserFacingDeviceProfileOptions;

	// Does the platform support granular video quality settings?
	UPROPERTY(EditAnywhere, Config, Category=VideoSettings)
	bool bSupportsGranularVideoQualitySettings = true;

	// Does the platform support running the automatic quality benchmark (typically this should only be true if bSupportsGranularVideoQualitySettings is also true)
	UPROPERTY(EditAnywhere, Config, Category=VideoSettings)
	bool bSupportsAutomaticVideoQualityBenchmark = true;

	// How is frame pacing controlled
	UPROPERTY(EditAnywhere, Config, Category=VideoSettings)
	ELochFramePacingMode FramePacingMode = ELochFramePacingMode::DesktopStyle;

	// What are the possible frame rates (for mobile style)?
	UPROPERTY(EditAnywhere, Config, Category=VideoSettings, meta=(EditCondition="FramePacingMode==ELochFramePacingMode::MobileStyle"))
	TArray<int32> MobileFrameRateLimits;
};

UCLASS(config=Game, defaultconfig, meta=(DisplayName="Loch Performance Settings"))
class ULochPerformanceSettings : public UDeveloperSettingsBackedByCVars
{
	GENERATED_BODY()

public:
	ULochPerformanceSettings();

private:
	// The console variables (initial values) to set from this config (these are set at startup, but can be overridden by the user)
	//
	// Note: This needs to be 'per-platform' so we can do things like disable certain features on specific platforms
	UPROPERTY(EditAnywhere, Config, Category=Performance, meta=(ShowOnlyInnerProperties))
	FPerPlatformSettings PerPlatformSettings;
	
public:
	// The list of frame rates to verify are supported by the monitor (and offer in the UI)
	UPROPERTY(EditAnywhere, Config, Category=Performance)
	TArray<int32> DesktopFrameRateLimits;

	// The list of performance stats that can be enabled in the UI
	UPROPERTY(EditAnywhere, Config, Category=Performance)
	TArray<FLochPerformanceStatGroup> UserFacingPerformanceStats;
};
