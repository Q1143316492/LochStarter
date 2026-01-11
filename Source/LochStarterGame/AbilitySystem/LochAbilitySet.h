// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ActiveGameplayEffectHandle.h"
#include "Engine/DataAsset.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"

#include "GameplayAbilitySpecHandle.h"
#include "LochAbilitySet.generated.h"

class UAttributeSet;
class UGameplayEffect;
class ULochAbilitySystemComponent;
class ULochGameplayAbility;
class UObject;


/**
 * FLochAbilitySet_GameplayAbility
 *
 *	能力集用于授予游戏玩法能力的数据。
 */
USTRUCT(BlueprintType)
struct FLochAbilitySet_GameplayAbility
{
	GENERATED_BODY()

public:

	// 要赋予的游戏玩法能力。
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ULochGameplayAbility> Ability = nullptr;

	// 要赋予的能力等级。
	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;

	// 用于处理能力输入的标签。
	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};


/**
 * FLochAbilitySet_GameplayEffect
 *
 *	能力集用于授予游戏玩法效果的数据。
 */
USTRUCT(BlueprintType)
struct FLochAbilitySet_GameplayEffect
{
	GENERATED_BODY()

public:

	// 要赋予的游戏玩法效果。
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect = nullptr;

	// 要赋予的游戏玩法效果等级。
	UPROPERTY(EditDefaultsOnly)
	float EffectLevel = 1.0f;
};

/**
 * FLochAbilitySet_AttributeSet
 *
 *	能力集用于授予属性集的数据。
 */
USTRUCT(BlueprintType)
struct FLochAbilitySet_AttributeSet
{
	GENERATED_BODY()

public:
	// Gameplay effect to grant.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAttributeSet> AttributeSet;

};

/**
 * FLochAbilitySet_GrantedHandles
 *
 *	用于存储能力集所授予内容的句柄的数据
 */
USTRUCT(BlueprintType)
struct FLochAbilitySet_GrantedHandles
{
	GENERATED_BODY()

public:

	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);
	void AddAttributeSet(UAttributeSet* Set);

	void TakeFromAbilitySystem(ULochAbilitySystemComponent* LochASC);

protected:

	// 句柄，指向已授予的能力。
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	// 句柄，指向已授予的游戏玩法效果。
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;

	// 指向已授予的属性集的指针
	UPROPERTY()
	TArray<TObjectPtr<UAttributeSet>> GrantedAttributeSets;
};


/**
 * ULochAbilitySet
 *
 *	用于授予游戏玩法能力和游戏玩法效果的不可变数据资产。
 */
UCLASS(BlueprintType, Const)
class ULochAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	ULochAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// 向指定的能力系统组件授予能力集。
	// 返回的句柄可在之后用于移除所有已授予的内容。
	void GiveToAbilitySystem(ULochAbilitySystemComponent* LochASC, FLochAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr) const;

protected:

	// GA = ULochGameplayAbility + int32 AbilityLevel + FGameplayTag InputTag
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", meta=(TitleProperty=Ability))
	TArray<FLochAbilitySet_GameplayAbility> GrantedGameplayAbilities;

	// GE = UGameplayEffect + float EffectLevel
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects", meta=(TitleProperty=GameplayEffect))
	TArray<FLochAbilitySet_GameplayEffect> GrantedGameplayEffects;

	// AS = UAttributeSet
	UPROPERTY(EditDefaultsOnly, Category = "Attribute Sets", meta=(TitleProperty=AttributeSet))
	TArray<FLochAbilitySet_AttributeSet> GrantedAttributes;
};
