#pragma once

#include "Engine/DataAsset.h"
#include "LochExperienceActionSet.generated.h"

class UGameFeatureAction;


UCLASS(BlueprintType, NotBlueprintable)
class ULochExperienceActionSet : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    ULochExperienceActionSet();

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

#if WITH_EDITORONLY_DATA
	virtual void UpdateAssetBundleData() override;
#endif

public:
	UPROPERTY(EditAnywhere, Instanced, Category="Actions to Perform")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;

	UPROPERTY(EditAnywhere, Category="Feature Dependencies")
	TArray<FString> GameFeaturesToEnable;

};