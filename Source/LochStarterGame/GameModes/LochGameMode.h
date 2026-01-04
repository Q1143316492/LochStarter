#pragma once

#include "ModularGameMode.h"
#include "LochGameMode.generated.h"

#define UE_API LOCHSTARTERGAME_API

class AActor;
class AController;
class AGameModeBase;
class APawn;
class APlayerController;
class UClass;
class ULochExperienceDefinition;
class ULochPawnData;
class UObject;
struct FFrame;
struct FPrimaryAssetId;
enum class ECommonSessionOnlineMode : uint8;


/**
 * ALochGameMode
 *
 *	The base game mode class used by this project.
 */
UCLASS(MinimalAPI, Config = Game, Meta = (ShortTooltip = "The base game mode class used by this project."))
class ALochGameMode : public AModularGameModeBase
{
	GENERATED_BODY()
public:

	UE_API ALochGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};

#undef UE_API