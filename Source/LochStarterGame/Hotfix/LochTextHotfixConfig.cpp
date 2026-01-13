// Copyright Epic Games, Inc. All Rights Reserved.

#include "LochTextHotfixConfig.h"
#include "Internationalization/PolyglotTextData.h"
#include "Internationalization/TextLocalizationManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LochTextHotfixConfig)

ULochTextHotfixConfig::ULochTextHotfixConfig(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ULochTextHotfixConfig::ApplyTextReplacements() const
{
	FTextLocalizationManager::Get().RegisterPolyglotTextData(TextReplacements);
}

void ULochTextHotfixConfig::PostInitProperties()
{
	Super::PostInitProperties();
	ApplyTextReplacements();
}

void ULochTextHotfixConfig::PostReloadConfig(FProperty* PropertyThatWasLoaded)
{
	Super::PostReloadConfig(PropertyThatWasLoaded);
	ApplyTextReplacements();
}

#if WITH_EDITOR
void ULochTextHotfixConfig::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	ApplyTextReplacements();
}
#endif

