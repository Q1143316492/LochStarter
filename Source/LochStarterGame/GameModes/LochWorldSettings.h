// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/WorldSettings.h"
#include "LochWorldSettings.generated.h"

#define UE_API LOCHSTARTERGAME_API

class ULochExperienceDefinition;


UCLASS(MinimalAPI)
class ALochWorldSettings : public AWorldSettings
{
	GENERATED_BODY()

public:
	UE_API ALochWorldSettings(const FObjectInitializer& ObjectInitializer);

#if WITH_EDITOR
	UE_API virtual void CheckForErrors() override;
#endif

    UE_API FPrimaryAssetId GetDefaultGameplayExperience() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category=GameMode)
	TSoftClassPtr<ULochExperienceDefinition> DefaultGameplayExperience;

public:
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category=PIE)
	bool ForceStandaloneNetMode = false;
#endif
};

#undef UE_API