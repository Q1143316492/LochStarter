// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayEffectTypes.h"

#include "LochGameplayEffectContext.generated.h"

class AActor;
class FArchive;
class ILochAbilitySourceInterface;
class UObject;
class UPhysicalMaterial;

USTRUCT()
struct FLochGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

	FLochGameplayEffectContext()
		: FGameplayEffectContext()
	{
	}

	FLochGameplayEffectContext(AActor* InInstigator, AActor* InEffectCauser)
		: FGameplayEffectContext(InInstigator, InEffectCauser)
	{
	}

	/** Returns the wrapped FLochGameplayEffectContext from the handle, or nullptr if it doesn't exist or is the wrong type */
	static LOCHSTARTERGAME_API FLochGameplayEffectContext* ExtractEffectContext(struct FGameplayEffectContextHandle Handle);

	/** Sets the object used as the ability source */
	void SetAbilitySource(const ILochAbilitySourceInterface* InObject, float InSourceLevel);

	/** Returns the ability source interface associated with the source object. Only valid on the authority. */
	const ILochAbilitySourceInterface* GetAbilitySource() const;

	virtual FGameplayEffectContext* Duplicate() const override
	{
		FLochGameplayEffectContext* NewContext = new FLochGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FLochGameplayEffectContext::StaticStruct();
	}

	/** Overridden to serialize new fields */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

	/** 
     * Returns the physical material from the hit result if there is one 
     * 如果有命中结果，则返回物理材质
     */
	const UPhysicalMaterial* GetPhysicalMaterial() const;

public:
	/** 
     * ID to allow the identification of multiple bullets that were part of the same cartridge 
     * 来自同一弹夹的多发子弹的识别 ID
     * */
	UPROPERTY()
	int32 CartridgeID = -1;

protected:
	/** 
     * Ability Source object (should implement ILochAbilitySourceInterface). NOT replicated currently 
     * 能力源对象（应实现 ILochAbilitySourceInterface）。当前未复制
     * */
	UPROPERTY()
	TWeakObjectPtr<const UObject> AbilitySourceObject;
};

template<>
struct TStructOpsTypeTraits<FLochGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FLochGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};
