// Copyright Epic Games, Inc. All Rights Reserved.

#include "LochGameplayEffectContext.h"

#include "AbilitySystem/LochAbilitySourceInterface.h"
#include "Engine/HitResult.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

#include "Iris/ReplicationState/PropertyNetSerializerInfoRegistry.h"
#include "Serialization/GameplayEffectContextNetSerializer.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LochGameplayEffectContext)

class FArchive;

FLochGameplayEffectContext* FLochGameplayEffectContext::ExtractEffectContext(struct FGameplayEffectContextHandle Handle)
{
	FGameplayEffectContext* BaseEffectContext = Handle.Get();
	if ((BaseEffectContext != nullptr) && BaseEffectContext->GetScriptStruct()->IsChildOf(FLochGameplayEffectContext::StaticStruct()))
	{
		return (FLochGameplayEffectContext*)BaseEffectContext;
	}

	return nullptr;
}

bool FLochGameplayEffectContext::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	FGameplayEffectContext::NetSerialize(Ar, Map, bOutSuccess);

	// Not serialized for post-activation use:
	// CartridgeID

	return true;
}

namespace UE::Net
{
	// Forward to FGameplayEffectContextNetSerializer
	// Note: If FLorchGameplayEffectContext::NetSerialize() is modified, a custom NetSerializer must be implemented as the current fallback will no longer be sufficient.
	UE_NET_IMPLEMENT_FORWARDING_NETSERIALIZER_AND_REGISTRY_DELEGATES(LochGameplayEffectContext, FGameplayEffectContextNetSerializer);
}

void FLochGameplayEffectContext::SetAbilitySource(const ILochAbilitySourceInterface* InObject, float InSourceLevel)
{
	AbilitySourceObject = MakeWeakObjectPtr(Cast<const UObject>(InObject));
	//SourceLevel = InSourceLevel;
}

const ILochAbilitySourceInterface* FLochGameplayEffectContext::GetAbilitySource() const
{
	return Cast<ILochAbilitySourceInterface>(AbilitySourceObject.Get());
}

const UPhysicalMaterial* FLochGameplayEffectContext::GetPhysicalMaterial() const
{
	if (const FHitResult* HitResultPtr = GetHitResult())
	{
		return HitResultPtr->PhysMaterial.Get();
	}
	return nullptr;
}
