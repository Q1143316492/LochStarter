#pragma once

#include "AbilitySystemComponent.h"
#include "NativeGameplayTags.h"

#include "LochAbilitySystemComponent.generated.h"

#define UE_API LOCHSTARTERGAME_API

class AActor;
class UGameplayAbility;
class UObject;
struct FFrame;
struct FGameplayAbilityTargetDataHandle;

/**
 * ULochAbilitySystemComponent
 *
 *	Base ability system component class used by this project.
 */
UCLASS(MinimalAPI)
class ULochAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	UE_API ULochAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};

#undef UE_API