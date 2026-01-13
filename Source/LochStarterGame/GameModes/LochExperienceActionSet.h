#pragma once

#include "Engine/DataAsset.h"
#include "LochExperienceActionSet.generated.h"

class UGameFeatureAction;


/*
 * 是一个数据资产，包含一组 UGameFeatureAction 对象，这些对象定义了在特定游戏体验中应执行的操作。
 * 还包含一个字符串数组，指定在体验期间应启用的游戏功能。
 * 为了逻辑分离吧
 */
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