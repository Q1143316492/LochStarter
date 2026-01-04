#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "NativeGameplayTags.h"
#include "LochCharacterMovementComponent.generated.h"


#define UE_API LOCHSTARTERGAME_API

/**
 * ULochCharacterMovementComponent
 *
 *	The base character movement component class used by this project.
 */
UCLASS(MinimalAPI, Config = Game)
class ULochCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UE_API ULochCharacterMovementComponent(const FObjectInitializer& ObjectInitializer);
};


#undef UE_API