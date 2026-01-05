#pragma once

#include "GameFramework/GameUserSettings.h"
#include "InputCoreTypes.h"

#include "LochSettingsLocal.generated.h"

enum class ECommonInputType : uint8;
enum class ELochDisplayablePerformanceStat : uint8;
enum class ELochStatDisplayMode : uint8;

class ULochLocalPlayer;
class UObject;
class USoundControlBus;
class USoundControlBusMix;
struct FFrame;

USTRUCT()
struct FLochScalabilitySnapshot
{
	GENERATED_BODY()

	FLochScalabilitySnapshot();
	Scalability::FQualityLevels Qualities;
	bool bActive = false;
	bool bHasOverrides = false;
};

UCLASS()
class ULochSettingsLocal : public UGameUserSettings
{
	GENERATED_BODY()

public:

	ULochSettingsLocal();

	static ULochSettingsLocal* Get();

};