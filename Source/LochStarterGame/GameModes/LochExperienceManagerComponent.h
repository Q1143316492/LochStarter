#pragma once

#include "Components/GameStateComponent.h"
#include "LoadingProcessInterface.h"

class ULochExperienceDefinition;

#include "LochExperienceManagerComponent.generated.h"

#define UE_API LOCHSTARTERGAME_API

namespace UE::GameFeatures { struct FResult; }

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLochExperienceLoaded, const ULochExperienceDefinition* /*Experience*/);

enum class ELochExperienceLoadState
{
	Unloaded,
	Loading,
	LoadingGameFeatures,
	LoadingChaosTestingDelay,
	ExecutingActions,
	Loaded,
	Deactivating
};

UCLASS(MinimalAPI)
class ULochExperienceManagerComponent final : public UGameStateComponent, public ILoadingProcessInterface
{
	GENERATED_BODY()

public:
	UE_API ULochExperienceManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UActorComponent interface
	UE_API virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of UActorComponent interface

	//~ILoadingProcessInterface interface
	UE_API virtual bool ShouldShowLoadingScreen(FString& OutReason) const override;
	//~End of ILoadingProcessInterface

	UE_API void SetCurrentExperience(FPrimaryAssetId ExperienceId);

	UE_API void CallOrRegister_OnExperienceLoaded_HighPriority(FOnLochExperienceLoaded::FDelegate&& Delegate);
	UE_API void CallOrRegister_OnExperienceLoaded(FOnLochExperienceLoaded::FDelegate&& Delegate);
	UE_API void CallOrRegister_OnExperienceLoaded_LowPriority(FOnLochExperienceLoaded::FDelegate&& Delegate);

	UE_API const ULochExperienceDefinition* GetCurrentExperienceChecked() const;

	UE_API bool IsExperienceLoaded() const;

private:
	UFUNCTION()
	void OnRep_CurrentExperience();

	void StartExperienceLoad();
	void OnExperienceLoadComplete();
	void OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result);
	void OnExperienceFullLoadCompleted();

	void OnActionDeactivationCompleted();
	void OnAllActionsDeactivated();

	UPROPERTY(ReplicatedUsing=OnRep_CurrentExperience)
	TObjectPtr<const ULochExperienceDefinition> CurrentExperience;

	ELochExperienceLoadState LoadState = ELochExperienceLoadState::Unloaded;

	int32 NumGameFeaturePluginsLoading = 0;
	TArray<FString> GameFeaturePluginURLs;

	int32 NumObservedPausers = 0; // 当前多少
	int32 NumExpectedPausers = 0; // 预计多少，判断是否全部完成

	FOnLochExperienceLoaded OnExperienceLoaded_HighPriority;
	FOnLochExperienceLoaded OnExperienceLoaded;
	FOnLochExperienceLoaded OnExperienceLoaded_LowPriority;
};

#undef UE_API