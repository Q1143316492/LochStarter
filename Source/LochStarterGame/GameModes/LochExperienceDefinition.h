#pragma once

#include "Engine/DataAsset.h"
#include "LochExperienceDefinition.generated.h"

class UGameFeatureAction;
class ULochPawnData;
class ULochExperienceActionSet;

/**
 * Definition of an experience
 */
UCLASS(BlueprintType, Const)
class LOCHSTARTERGAME_API ULochExperienceDefinition : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    ULochExperienceDefinition();

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

public:
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TArray<FString> GameFeaturesToEnable;

	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TObjectPtr<const ULochPawnData> DefaultPawnData;

    UPROPERTY(EditDefaultsOnly, Instanced, Category="Actions")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;

    UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TArray<TObjectPtr<ULochExperienceActionSet>> ActionSets;
};