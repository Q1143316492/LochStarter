// Copyright Epic Games, Inc. All Rights Reserved.

#include "LochInventoryItemDefinition.h"

#include "Templates/SubclassOf.h"
#include "UObject/ObjectPtr.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LochInventoryItemDefinition)

//////////////////////////////////////////////////////////////////////
// ULochInventoryItemDefinition

ULochInventoryItemDefinition::ULochInventoryItemDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

const ULochInventoryItemFragment* ULochInventoryItemDefinition::FindFragmentByClass(TSubclassOf<ULochInventoryItemFragment> FragmentClass) const
{
	if (FragmentClass != nullptr)
	{
		for (ULochInventoryItemFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}

	return nullptr;
}

//////////////////////////////////////////////////////////////////////
// ULochInventoryItemDefinition

const ULochInventoryItemFragment* ULochInventoryFunctionLibrary::FindItemDefinitionFragment(TSubclassOf<ULochInventoryItemDefinition> ItemDef, TSubclassOf<ULochInventoryItemFragment> FragmentClass)
{
	if ((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		return GetDefault<ULochInventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}
	return nullptr;
}

