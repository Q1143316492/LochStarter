// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"
#include "Abilities/GameplayAbility.h"
#include "LochAbilityCost.generated.h"

class ULochGameplayAbility;

/**
 * ULochAbilityCost
 *
 * LochGameplayAbility 所拥有的消耗（例如弹药或次数）的基类
 */
UCLASS(MinimalAPI, DefaultToInstanced, EditInlineNew, Abstract)
class ULochAbilityCost : public UObject
{
	GENERATED_BODY()

public:
	ULochAbilityCost()
	{
	}

	/**
	 * 检查我们是否能承担这笔费用。
	 *
	 * 可以将失败原因标签添加到 OptionalRelevantTags（如果非空），可以在其他地方查询
	 * 以确定如何提供用户反馈（例如，如果武器弹药耗尽，则发出点击声）
	 * 
	 * 进入时，Ability 和 ActorInfo 保证非空，但 OptionalRelevantTags 可以为空。
	 * 
	 * @return 如果我们能支付能力费用，则返回 true，否则返回 false。
	 */
	virtual bool CheckCost(const ULochGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
	{
		return true;
	}

	/**
	 * 将该能力的消耗施加于目标
	 *
	 * 注意：
	 * - 你的实现不需要检查 ShouldOnlyApplyCostOnHit ()，调用者会为你做这件事。
 	 * - 进入时，Ability 和 ActorInfo 保证非空。
	 */
	virtual void ApplyCost(const ULochGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
	{
	}

	/** 如果为 true，则仅当该能力成功命中时才应应用此消耗 */
	bool ShouldOnlyApplyCostOnHit() const { return bOnlyApplyCostOnHit; }

protected:
	/** 如果为 true，则仅当该能力成功命中时才应应用此消耗 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Costs)
	bool bOnlyApplyCostOnHit = false;
};
