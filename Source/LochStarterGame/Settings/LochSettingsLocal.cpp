// Copyright Epic Games, Inc. All Rights Reserved.

#include "Settings/LochSettingsLocal.h"
#include "Engine/Engine.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LochSettingsLocal)

#if WITH_EDITOR
static TAutoConsoleVariable<bool> CVarApplyFrameRateSettingsInPIE(TEXT("Loch.Settings.ApplyFrameRateSettingsInPIE"),
	false,
	TEXT("Should we apply frame rate settings in PIE?"),
	ECVF_Default);

static TAutoConsoleVariable<bool> CVarApplyFrontEndPerformanceOptionsInPIE(TEXT("Loch.Settings.ApplyFrontEndPerformanceOptionsInPIE"),
	false,
	TEXT("Do we apply front-end specific performance options in PIE?"),
	ECVF_Default);

static TAutoConsoleVariable<bool> CVarApplyDeviceProfilesInPIE(TEXT("Loch.Settings.ApplyDeviceProfilesInPIE"),
	false,
	TEXT("Should we apply experience/platform emulated device profiles in PIE?"),
	ECVF_Default);
#endif

FLochScalabilitySnapshot::FLochScalabilitySnapshot()
{
	static_assert(sizeof(Scalability::FQualityLevels) == 88, "This function may need to be updated to account for new members");

	Qualities.ResolutionQuality = -1.0f;
	Qualities.ViewDistanceQuality = -1;
	Qualities.AntiAliasingQuality = -1;
	Qualities.ShadowQuality = -1;
	Qualities.GlobalIlluminationQuality = -1;
	Qualities.ReflectionQuality = -1;
	Qualities.PostProcessQuality = -1;
	Qualities.TextureQuality = -1;
	Qualities.EffectsQuality = -1;
	Qualities.FoliageQuality = -1;
	Qualities.ShadingQuality = -1;
}

ULochSettingsLocal::ULochSettingsLocal()
{
}

ULochSettingsLocal* ULochSettingsLocal::Get()
{
	return GEngine ? CastChecked<ULochSettingsLocal>(GEngine->GetGameUserSettings()) : nullptr;
}

void ULochSettingsLocal::BeginDestroy()
{
	Super::BeginDestroy();
}

void ULochSettingsLocal::SetToDefaults()
{
	Super::SetToDefaults();
}

void ULochSettingsLocal::LoadSettings(bool bForceReload)
{
	Super::LoadSettings(bForceReload);
}

void ULochSettingsLocal::ConfirmVideoMode()
{
	Super::ConfirmVideoMode();
}

float ULochSettingsLocal::GetEffectiveFrameRateLimit()
{
	return Super::GetEffectiveFrameRateLimit();
}

void ULochSettingsLocal::ResetToCurrentSettings()
{
	Super::ResetToCurrentSettings();
}

void ULochSettingsLocal::ApplyNonResolutionSettings()
{
	Super::ApplyNonResolutionSettings();
}

int32 ULochSettingsLocal::GetOverallScalabilityLevel() const
{
	return Super::GetOverallScalabilityLevel();
}

void ULochSettingsLocal::SetOverallScalabilityLevel(int32 Value)
{
	Super::SetOverallScalabilityLevel(Value);
}

