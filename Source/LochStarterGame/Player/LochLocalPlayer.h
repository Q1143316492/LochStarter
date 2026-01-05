#pragma once

#include "CommonLocalPlayer.h"

#include "LochLocalPlayer.generated.h"

#define UE_API LOCHSTARTERGAME_API

struct FGenericTeamId;

class APlayerController;
class UInputMappingContext;
// class ULochSettingsLocal;
// class ULochSettingsShared;
class UObject;
class UWorld;
struct FFrame;
struct FSwapAudioOutputResult;

/**
 * ULochLocalPlayer
 */
UCLASS(MinimalAPI)
class ULochLocalPlayer : public UCommonLocalPlayer
{
	GENERATED_BODY()

public:

	UE_API ULochLocalPlayer();

};


#undef UE_API