// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "LochAbilityCost.h"
#include "ScalableFloat.h"
#include "Templates/SubclassOf.h"

#include "LochAbilityCost_InventoryItem.generated.h"

struct FGameplayAbilityActivationInfo;
struct FGameplayAbilitySpecHandle;

class ULochGameplayAbility;
class ULochInventoryItemDefinition;
class UObject;
struct FGameplayAbilityActorInfo;
struct FGameplayTagContainer;

/**
 * 表示一种需要消耗一定数量库存物品的成本
 */
UCLASS(meta=(DisplayName="Inventory Item"))
class ULochAbilityCost_InventoryItem : public ULochAbilityCost
{
	GENERATED_BODY()

public:
	ULochAbilityCost_InventoryItem();

	//~ULochAbilityCost interface
	virtual bool CheckCost(const ULochGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ApplyCost(const ULochGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	//~End of ULochAbilityCost interface

protected:
	/** 该物品要花费多少（根据能力水平确定） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=AbilityCost)
	FScalableFloat Quantity;

	/** 该物品要消耗的类型 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=AbilityCost)
	TSubclassOf<ULochInventoryItemDefinition> ItemDefinition;
};
