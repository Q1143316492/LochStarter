// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h"
#include "UObject/WeakInterfacePtr.h"

#include "LoadingScreenManager.generated.h"

#define UE_API COMMONLOADINGSCREEN_API

template <typename InterfaceType> class TScriptInterface;

class FSubsystemCollectionBase;
class IInputProcessor;
class ILoadingProcessInterface;
class SWidget;
class UObject;
class UWorld;
struct FFrame;
struct FWorldContext;

/**
 * Handles showing/hiding the loading screen
 * 负责显示/隐藏加载界面
 */
UCLASS(MinimalAPI)
class ULoadingScreenManager : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	//~USubsystem interface
	UE_API virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	UE_API virtual void Deinitialize() override;
	UE_API virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	//~End of USubsystem interface

	//~FTickableObjectBase interface
	UE_API virtual void Tick(float DeltaTime) override;
	UE_API virtual ETickableTickType GetTickableTickType() const override;
	UE_API virtual bool IsTickable() const override;
	UE_API virtual TStatId GetStatId() const override;
	UE_API virtual UWorld* GetTickableGameObjectWorld() const override;
	//~End of FTickableObjectBase interface

	UFUNCTION(BlueprintCallable, Category=LoadingScreen)
	FString GetDebugReasonForShowingOrHidingLoadingScreen() const
	{
		return DebugReasonForShowingOrHidingLoadingScreen;
	}

	/** Returns True when the loading screen is currently being shown */
	/** 当加载界面当前正在显示时返回True */
	bool GetLoadingScreenDisplayStatus() const
	{
		return bCurrentlyShowingLoadingScreen;
	}

	/** Called when the loading screen visibility changes  */
	/** 当加载界面可见性发生变化时调用 */
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnLoadingScreenVisibilityChangedDelegate, bool);
	FORCEINLINE FOnLoadingScreenVisibilityChangedDelegate& OnLoadingScreenVisibilityChangedDelegate() { return LoadingScreenVisibilityChanged; }

	UE_API void RegisterLoadingProcessor(TScriptInterface<ILoadingProcessInterface> Interface);
	UE_API void UnregisterLoadingProcessor(TScriptInterface<ILoadingProcessInterface> Interface);
	
private:
	UE_API void HandlePreLoadMap(const FWorldContext& WorldContext, const FString& MapName);
	UE_API void HandlePostLoadMap(UWorld* World);

	/** Determines if we should show or hide the loading screen. Called every frame. */
	/** 判断是否应该显示或隐藏加载界面。每帧调用。*/
	UE_API void UpdateLoadingScreen();

	/** Returns true if we need to be showing the loading screen. */
	/** 如果需要显示加载界面则返回true。*/
	UE_API bool CheckForAnyNeedToShowLoadingScreen();

	/** Returns true if we want to be showing the loading screen (if we need to or are artificially forcing it on for other reasons). */
	/** 如果我们需要或由于其他原因强制显示加载界面则返回true。*/
	UE_API bool ShouldShowLoadingScreen();

	/** Returns true if we are in the initial loading flow before this screen should be used */
	/** 如果还处于初始加载流程且尚未使用此界面则返回true。*/
	UE_API bool IsShowingInitialLoadingScreen() const;

	/** Shows the loading screen. Sets up the loading screen widget on the viewport */
	/** 显示加载界面。在视口上设置加载界面控件。*/
	UE_API void ShowLoadingScreen();

	/** Hides the loading screen. The loading screen widget will be destroyed */
	/** 隐藏加载界面。加载界面控件将被销毁。*/
	UE_API void HideLoadingScreen();

	/** Removes the widget from the viewport */
	/** 从视口移除控件。*/
	UE_API void RemoveWidgetFromViewport();

	/** Prevents input from being used in-game while the loading screen is visible */
	/** 当加载界面可见时阻止游戏内输入。*/
	UE_API void StartBlockingInput();

	/** Resumes in-game input, if blocking */
	/** 恢复游戏内输入（如果之前被阻止）。*/
	UE_API void StopBlockingInput();

	UE_API void ChangePerformanceSettings(bool bEnabingLoadingScreen);

private:
	/** Delegate broadcast when the loading screen visibility changes */
	/** 当加载界面可见性变化时广播的委托。*/
	FOnLoadingScreenVisibilityChangedDelegate LoadingScreenVisibilityChanged;

	/** A reference to the loading screen widget we are displaying (if any) */
	/** 当前显示的加载界面控件的引用（如有）。*/
	TSharedPtr<SWidget> LoadingScreenWidget;

	/** Input processor to eat all input while the loading screen is shown */
	/** 加载界面显示时吞掉所有输入的输入处理器。*/
	TSharedPtr<IInputProcessor> InputPreProcessor;

	/** External loading processors, components maybe actors that delay the loading. */
	/** 外部加载处理器，可能是延迟加载的组件或Actor。*/
	TArray<TWeakInterfacePtr<ILoadingProcessInterface>> ExternalLoadingProcessors;

	/** The reason why the loading screen is up (or not) */
	/** 加载界面显示（或未显示）的原因。*/
	FString DebugReasonForShowingOrHidingLoadingScreen;

	/** The time when we started showing the loading screen */
	/** 开始显示加载界面的时间。*/
	double TimeLoadingScreenShown = 0.0;

	/** The time the loading screen most recently wanted to be dismissed (might still be up due to a min display duration requirement) **/
	/** 最近一次希望关闭加载界面的时间（可能因最小显示时长要求仍在显示）。*/
	double TimeLoadingScreenLastDismissed = -1.0;

	/** The time until the next log for why the loading screen is still up */
	/** 距离下次记录加载界面为何仍在显示的日志的时间。*/
	double TimeUntilNextLogHeartbeatSeconds = 0.0;

	/** True when we are between PreLoadMap and PostLoadMap */
	/** 处于PreLoadMap和PostLoadMap之间时为true。*/
	bool bCurrentlyInLoadMap = false;

	/** True when the loading screen is currently being shown */
	/** 当前加载界面正在显示时为true。*/
	bool bCurrentlyShowingLoadingScreen = false;
};

#undef UE_API
