// Copyright Epic Games, Inc. All Rights Reserved.

#include "LochAbilityCost_PlayerTagStack.h"

#include "GameFramework/Controller.h"
#include "LochGameplayAbility.h"
#include "Player/LochPlayerState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LochAbilityCost_PlayerTagStack)

ULochAbilityCost_PlayerTagStack::ULochAbilityCost_PlayerTagStack()
{
	Quantity.SetValue(1.0f);
}

bool ULochAbilityCost_PlayerTagStack::CheckCost(const ULochGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (AController* PC = Ability->GetControllerFromActorInfo())
	{
		if (ALochPlayerState* PS = Cast<ALochPlayerState>(PC->PlayerState))
		{
			const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);

			const float NumStacksReal = Quantity.GetValueAtLevel(AbilityLevel);
			const int32 NumStacks = FMath::TruncToInt(NumStacksReal);

			return PS->GetStatTagStackCount(Tag) >= NumStacks;
		}
	}
	return false;
}

void ULochAbilityCost_PlayerTagStack::ApplyCost(const ULochGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ActorInfo->IsNetAuthority())
	{
		if (AController* PC = Ability->GetControllerFromActorInfo())
		{
			if (ALochPlayerState* PS = Cast<ALochPlayerState>(PC->PlayerState))
			{
				const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);

				const float NumStacksReal = Quantity.GetValueAtLevel(AbilityLevel);
				const int32 NumStacks = FMath::TruncToInt(NumStacksReal);

				PS->RemoveStatTagStack(Tag, NumStacks);
			}
		}
	}
}

