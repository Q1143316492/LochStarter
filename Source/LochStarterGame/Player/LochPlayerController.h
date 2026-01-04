#pragma once

#include "CommonPlayerController.h"
#include "LochPlayerController.generated.h"


#define UE_API LOCHSTARTERGAME_API

/**
 * ALochPlayerController
 *
 *	The base player controller class used by this project.
 */
UCLASS(MinimalAPI, Config = Game, Meta = (ShortTooltip = "The base player controller class used by this project."))
class ALochPlayerController : public ACommonPlayerController
{
	GENERATED_BODY()
public:
	UE_API ALochPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};

// A player controller used for replay capture and playback
UCLASS()
class ALochReplayPlayerController : public ALochPlayerController
{
	GENERATED_BODY()

};

#undef UE_API