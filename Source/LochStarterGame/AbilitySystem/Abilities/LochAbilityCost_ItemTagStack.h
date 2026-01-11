// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "LochAbilityCost.h"
#include "ScalableFloat.h"

#include "LochAbilityCost_ItemTagStack.generated.h"

struct FGameplayAbilityActivationInfo;
struct FGameplayAbilitySpecHandle;

class ULochGameplayAbility;
class UObject;
struct FGameplayAbilityActorInfo;

/**
 * 表示一种需要消耗一定数量标签堆栈的成本
 * 在关联的物品实例上
 */
UCLASS(meta=(DisplayName="Item Tag Stack"))
class ULochAbilityCost_ItemTagStack : public ULochAbilityCost
{
	GENERATED_BODY()

public:
	ULochAbilityCost_ItemTagStack();

	//~ULochAbilityCost interface
	virtual bool CheckCost(const ULochGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ApplyCost(const ULochGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	//~End of ULochAbilityCost interface

protected:
	/** 该标签要消耗多少（根据能力水平确定） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Costs)
	FScalableFloat Quantity;

	/** 该标签要消耗多少（根据能力水平确定） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Costs)
	FGameplayTag Tag;

	/** 如果这笔费用无法适用，应返回哪个标签作为回应 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Costs)
	FGameplayTag FailureTag;
};
