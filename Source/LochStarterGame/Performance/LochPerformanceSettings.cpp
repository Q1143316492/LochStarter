// Copyright Epic Games, Inc. All Rights Reserved.

#include "LochPerformanceSettings.h"

#include "Engine/PlatformSettingsManager.h"
#include "Misc/EnumRange.h"
#include "Performance/LochPerformanceStatTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LochPerformanceSettings)

//////////////////////////////////////////////////////////////////////

ULochPlatformSpecificRenderingSettings::ULochPlatformSpecificRenderingSettings()
{
	MobileFrameRateLimits.Append({ 20, 30, 45, 60, 90, 120 });
}

const ULochPlatformSpecificRenderingSettings* ULochPlatformSpecificRenderingSettings::Get()
{
	ULochPlatformSpecificRenderingSettings* Result = UPlatformSettingsManager::Get().GetSettingsForPlatform<ThisClass>();
	check(Result);
	return Result;
}

//////////////////////////////////////////////////////////////////////

ULochPerformanceSettings::ULochPerformanceSettings()
{
	PerPlatformSettings.Initialize(ULochPlatformSpecificRenderingSettings::StaticClass());

	CategoryName = TEXT("Game");

	DesktopFrameRateLimits.Append({ 30, 60, 120, 144, 160, 165, 180, 200, 240, 360 });

	// Default to all stats are allowed
	FLochPerformanceStatGroup& StatGroup = UserFacingPerformanceStats.AddDefaulted_GetRef();
	for (ELochDisplayablePerformanceStat PerfStat : TEnumRange<ELochDisplayablePerformanceStat>())
	{
		StatGroup.AllowedStats.Add(PerfStat);
	}
}
