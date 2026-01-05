#pragma once

#include "Engine/DataAsset.h"

#include "LochGameData.generated.h"

#define UE_API LOCHSTARTERGAME_API

class UGameplayEffect;
class UObject;

/**
 * ULochGameData
 *
 *	Non-mutable data asset that contains global game data.
 */
UCLASS(MinimalAPI, BlueprintType, Const, Meta = (DisplayName = "Loch Game Data", ShortTooltip = "Data asset containing global game data."))
class ULochGameData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UE_API ULochGameData();

	static UE_API const ULochGameData& Get();

public:
	UPROPERTY(EditDefaultsOnly, Category = "Default Gameplay Effects", meta = (DisplayName = "Damage Gameplay Effect (SetByCaller)"))
	TSoftClassPtr<UGameplayEffect> DamageGameplayEffect_SetByCaller;

	UPROPERTY(EditDefaultsOnly, Category = "Default Gameplay Effects", meta = (DisplayName = "Heal Gameplay Effect (SetByCaller)"))
	TSoftClassPtr<UGameplayEffect> HealGameplayEffect_SetByCaller;

	UPROPERTY(EditDefaultsOnly, Category = "Default Gameplay Effects")
	TSoftClassPtr<UGameplayEffect> DynamicTagGameplayEffect;
};

#undef UE_API
