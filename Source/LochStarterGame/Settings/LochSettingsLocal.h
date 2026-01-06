// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/GameUserSettings.h"
#include "InputCoreTypes.h"
#include "LochSettingsLocal.generated.h"

class UObject;
class USoundControlBus;
class USoundControlBusMix;

USTRUCT()
struct FLochScalabilitySnapshot
{
	GENERATED_BODY()

	FLochScalabilitySnapshot();

	Scalability::FQualityLevels Qualities;
	bool bActive = false;
	bool bHasOverrides = false;
};

/**
 * ULochSettingsLocal
 */
UCLASS()
class ULochSettingsLocal : public UGameUserSettings
{
	GENERATED_BODY()

public:

	ULochSettingsLocal();

	static ULochSettingsLocal* Get();

	//~UObject interface
	virtual void BeginDestroy() override;
	//~End of UObject interface

	//~UGameUserSettings interface
	virtual void SetToDefaults() override;
	virtual void LoadSettings(bool bForceReload) override;
	virtual void ConfirmVideoMode() override;
	virtual float GetEffectiveFrameRateLimit() override;
	virtual void ResetToCurrentSettings() override;
	virtual void ApplyNonResolutionSettings() override;
	virtual int32 GetOverallScalabilityLevel() const override;
	virtual void SetOverallScalabilityLevel(int32 Value) override;
	//~End of UGameUserSettings interface
	
	DECLARE_EVENT_OneParam(ULochSettingsLocal, FAudioDeviceChanged, const FString& /*DeviceId*/);
	FAudioDeviceChanged OnAudioOutputDeviceChanged;

	// TODO: Performance stats
	
	// TODO: Display Gamma

	// TODO: Mobile Quality Settings
};
