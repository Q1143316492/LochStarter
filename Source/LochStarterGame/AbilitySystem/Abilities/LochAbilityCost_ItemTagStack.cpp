// Copyright Epic Games, Inc. All Rights Reserved.

#include "LochAbilityCost_ItemTagStack.h"

// #include "Equipment/LyraGameplayAbility_FromEquipment.h"
// #include "Inventory/LyraInventoryItemInstance.h"
#include "NativeGameplayTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LochAbilityCost_ItemTagStack)

UE_DEFINE_GAMEPLAY_TAG(TAG_ABILITY_FAIL_COST, "Ability.ActivateFail.Cost");

ULochAbilityCost_ItemTagStack::ULochAbilityCost_ItemTagStack()
{
	Quantity.SetValue(1.0f);
	FailureTag = TAG_ABILITY_FAIL_COST;
}

bool ULochAbilityCost_ItemTagStack::CheckCost(const ULochGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	// if (const ULochGameplayAbility_FromEquipment* EquipmentAbility = Cast<const ULochGameplayAbility_FromEquipment>(Ability))
	// {
	// 	if (ULochInventoryItemInstance* ItemInstance = EquipmentAbility->GetAssociatedItem())
	// 	{
	// 		const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);

	// 		const float NumStacksReal = Quantity.GetValueAtLevel(AbilityLevel);
	// 		const int32 NumStacks = FMath::TruncToInt(NumStacksReal);
	// 		const bool bCanApplyCost = ItemInstance->GetStatTagStackCount(Tag) >= NumStacks;

	// 		// Inform other abilities why this cost cannot be applied
	// 		if (!bCanApplyCost && OptionalRelevantTags && FailureTag.IsValid())
	// 		{
	// 			OptionalRelevantTags->AddTag(FailureTag);				
	// 		}
	// 		return bCanApplyCost;
	// 	}
	// }
	return false;
}

void ULochAbilityCost_ItemTagStack::ApplyCost(const ULochGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	// if (ActorInfo->IsNetAuthority())
	// {
	// 	if (const ULochGameplayAbility_FromEquipment* EquipmentAbility = Cast<const ULochGameplayAbility_FromEquipment>(Ability))
	// 	{
	// 		if (ULochInventoryItemInstance* ItemInstance = EquipmentAbility->GetAssociatedItem())
	// 		{
	// 			const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);

	// 			const float NumStacksReal = Quantity.GetValueAtLevel(AbilityLevel);
	// 			const int32 NumStacks = FMath::TruncToInt(NumStacksReal);

	// 			ItemInstance->RemoveStatTagStack(Tag, NumStacks);
	// 		}
	// 	}
	// }
}

