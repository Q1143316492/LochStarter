// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"

#include "LochAbilityTagRelationshipMapping.generated.h"

class UObject;

/** 
 * Struct that defines the relationship between different ability tags
 * 不同能力标签之间关系的结构体
 *  */
USTRUCT()
struct FLochAbilityTagRelationship
{
	GENERATED_BODY()

	/** 
     * The tag that this container relationship is about. Single tag, but abilities can have multiple of these 
     * 这个容器关系所涉及的标签。单个标签，但能力可以有多个这些标签
     * */
	UPROPERTY(EditAnywhere, Category = Ability, meta = (Categories = "Gameplay.Action"))
	FGameplayTag AbilityTag;

	/** 
     * The other ability tags that will be blocked by any ability using this tag
     * 使用此标签的任何能力将被阻止的其他能力标签
     *  */
	UPROPERTY(EditAnywhere, Category = Ability)
	FGameplayTagContainer AbilityTagsToBlock;

	/** 
     * The other ability tags that will be canceled by any ability using this tag
     * 使用此标签的任何能力将被取消的其他能力标签
     *  */
	UPROPERTY(EditAnywhere, Category = Ability)
	FGameplayTagContainer AbilityTagsToCancel;

	/** 
     * If an ability has the tag, this is implicitly added to the activation required tags of the ability 
     * 如果能力具有该标签，则会隐式添加到该能力的激活所需标签中
     * */
	UPROPERTY(EditAnywhere, Category = Ability)
	FGameplayTagContainer ActivationRequiredTags;

	/** 
     * If an ability has the tag, this is implicitly added to the activation blocked tags of the ability 
     * 如果能力具有该标签，则会隐式添加到该能力的激活阻止标签中
     * */
	UPROPERTY(EditAnywhere, Category = Ability)
	FGameplayTagContainer ActivationBlockedTags;
};


/** 
 * Mapping of how ability tags block or cancel other abilities 
 * 描述能力标签如何阻止或取消其他能力的映射
 * */
UCLASS()
class ULochAbilityTagRelationshipMapping : public UDataAsset
{
	GENERATED_BODY()

private:
	/** The list of relationships between different gameplay tags (which ones block or cancel others) */
	UPROPERTY(EditAnywhere, Category = Ability, meta=(TitleProperty="AbilityTag"))
	TArray<FLochAbilityTagRelationship> AbilityTagRelationships;

public:
	/** Given a set of ability tags, parse the tag relationship and fill out tags to block and cancel */
	void GetAbilityTagsToBlockAndCancel(const FGameplayTagContainer& AbilityTags, FGameplayTagContainer* OutTagsToBlock, FGameplayTagContainer* OutTagsToCancel) const;

	/** Given a set of ability tags, add additional required and blocking tags */
	void GetRequiredAndBlockedActivationTags(const FGameplayTagContainer& AbilityTags, FGameplayTagContainer* OutActivationRequired, FGameplayTagContainer* OutActivationBlocked) const;

	/** Returns true if the specified ability tags are canceled by the passed in action tag */
	bool IsAbilityCancelledByTag(const FGameplayTagContainer& AbilityTags, const FGameplayTag& ActionTag) const;
};
