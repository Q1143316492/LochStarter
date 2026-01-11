// Copyright Epic Games, Inc. All Rights Reserved.

#include "LochInputComponent.h"

#include "EnhancedInputSubsystems.h"
#include "Player/LochLocalPlayer.h"
#include "Settings/LochSettingsLocal.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LochInputComponent)

class ULochInputConfig;

ULochInputComponent::ULochInputComponent(const FObjectInitializer& ObjectInitializer)
{
}

void ULochInputComponent::AddInputMappings(const ULochInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	// Here you can handle any custom logic to add something from your input config if required
}

void ULochInputComponent::RemoveInputMappings(const ULochInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	// Here you can handle any custom logic to remove input mappings that you may have added above
}

void ULochInputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
	for (uint32 Handle : BindHandles)
	{
		RemoveBindingByHandle(Handle);
	}
	BindHandles.Reset();
}
