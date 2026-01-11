// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "LochInventoryItemDefinition.generated.h"

template <typename T> class TSubclassOf;

class ULochInventoryItemInstance;
struct FFrame;

//////////////////////////////////////////////////////////////////////

// 表示项目定义的一个片段
UCLASS(MinimalAPI, DefaultToInstanced, EditInlineNew, Abstract)
class ULochInventoryItemFragment : public UObject
{
	GENERATED_BODY()

public:
	virtual void OnInstanceCreated(ULochInventoryItemInstance* Instance) const {}
};

//////////////////////////////////////////////////////////////////////

/**
 * ULochInventoryItemDefinition
 */
UCLASS(Blueprintable, Const, Abstract)
class ULochInventoryItemDefinition : public UObject
{
	GENERATED_BODY()

public:
	ULochInventoryItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Display)
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Display, Instanced)
	TArray<TObjectPtr<ULochInventoryItemFragment>> Fragments;

public:
	const ULochInventoryItemFragment* FindFragmentByClass(TSubclassOf<ULochInventoryItemFragment> FragmentClass) const;
};

//@TODO: Make into a subsystem instead?
UCLASS()
class ULochInventoryFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, meta=(DeterminesOutputType=FragmentClass))
	static const ULochInventoryItemFragment* FindItemDefinitionFragment(TSubclassOf<ULochInventoryItemDefinition> ItemDef, TSubclassOf<ULochInventoryItemFragment> FragmentClass);
};
