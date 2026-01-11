// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "LochAbilityCost.h"
#include "ScalableFloat.h"

#include "LochAbilityCost_PlayerTagStack.generated.h"

struct FGameplayAbilityActivationInfo;
struct FGameplayAbilitySpecHandle;

class ULochGameplayAbility;
class UObject;
struct FGameplayAbilityActorInfo;

/**
 * 表示一种需要消耗一定数量标签堆栈的成本
 * 在玩家状态上
 */
UCLASS(meta=(DisplayName="Player Tag Stack"))
class ULochAbilityCost_PlayerTagStack : public ULochAbilityCost
{
	GENERATED_BODY()

public:
	ULochAbilityCost_PlayerTagStack();

	//~ULochAbilityCost interface
	virtual bool CheckCost(const ULochGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ApplyCost(const ULochGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	//~End of ULochAbilityCost interface

protected:
	/** 根据能力水平来决定要花费多少标签（以此为关键）*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Costs)
	FScalableFloat Quantity;

	/** 应该花一些在哪个标签上 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Costs)
	FGameplayTag Tag;
};
