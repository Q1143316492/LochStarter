// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/World.h"
#include "GameplayTagContainer.h"

#include "LochCameraMode.generated.h"

#define UE_API LOCHSTARTERGAME_API

class AActor;
class UCanvas;
class ULochCameraComponent;

/**
 * ELochCameraModeBlendFunction
 *
 *	用于相机模式之间过渡的混合函数。
 */
UENUM(BlueprintType)
enum class ELochCameraModeBlendFunction : uint8
{
	// 执行简单的线性插值。
	Linear,

	// 立即加速，但平滑地减速到目标。缓和量由指数控制。
	EaseIn,

	// 平滑加速，但不会平滑减速到目标。缓和量由指数控制。
	EaseOut,

	// 平滑加速和平滑减速。缓和量由指数控制。
	EaseInOut,

	COUNT	UMETA(Hidden)
};


/**
 * FLochCameraModeView
 *
 *	视图数据由相机模式生成，用于混合相机模式。
 */
struct FLochCameraModeView
{
public:

	FLochCameraModeView();
	void Blend(const FLochCameraModeView& Other, float OtherWeight);

public:

	FVector Location;
	FRotator Rotation;
	FRotator ControlRotation;
	float FieldOfView;
};


/**
 * ULochCameraMode
 *
 *	所有相机模式的基类。
 */
UCLASS(MinimalAPI, Abstract, NotBlueprintable)
class ULochCameraMode : public UObject
{
	GENERATED_BODY()

public:

	UE_API ULochCameraMode();

	UE_API ULochCameraComponent* GetLochCameraComponent() const;

	UE_API virtual UWorld* GetWorld() const override;

	UE_API AActor* GetTargetActor() const;

	const FLochCameraModeView& GetCameraModeView() const { return View; }

	virtual void OnActivation() {};
	virtual void OnDeactivation() {};

	UE_API void UpdateCameraMode(float DeltaTime);

	float GetBlendTime() const { return BlendTime; }
	float GetBlendWeight() const { return BlendWeight; }
	UE_API void SetBlendWeight(float Weight);

	FGameplayTag GetCameraTypeTag() const
	{
		return CameraTypeTag;
	}

	UE_API virtual void DrawDebug(UCanvas* Canvas) const;

protected:

	UE_API virtual FVector GetPivotLocation() const;
	UE_API virtual FRotator GetPivotRotation() const;

	UE_API virtual void UpdateView(float DeltaTime);
	UE_API virtual void UpdateBlending(float DeltaTime);

protected:
	// 一种可被游戏玩法代码查询的标签，当某种相机模式激活时，该代码会对此予以关注
	// 无需询问特定模式（例如，当通过瞄准镜瞄准时以获得更高精度）
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	FGameplayTag CameraTypeTag;

	// 由相机模式生成的视图输出。
	FLochCameraModeView View;

	// 水平视野（以度为单位）。
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "5.0", UIMax = "170", ClampMin = "5.0", ClampMax = "170.0"))
	float FieldOfView;

	// 最小视角仰角（单位：度）。
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMin;

	// 最大视角俯仰角（以度为单位）。
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMax;

	// 混合此模式所需的时间。
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendTime;

	// 用于混合的函数。
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	ELochCameraModeBlendFunction BlendFunction;

	// 指数用于混合函数以控制曲线的形状。
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendExponent;

	// 线性混合透明度用于确定混合权重。
	float BlendAlpha;

	// 混合权重由混合 alpha 和函数计算得出。
	float BlendWeight;

protected:
	/** 如果为 true，则跳过所有插值并将相机置于理想位置。下一帧会自动设为 false。 */
	UPROPERTY(transient)
	uint32 bResetInterpolation:1;
};


/**
 * ULochCameraModeStack
 *
 *	用于混合相机模式的堆栈。
 */
UCLASS()
class ULochCameraModeStack : public UObject
{
	GENERATED_BODY()

public:

	ULochCameraModeStack();

	void ActivateStack();
	void DeactivateStack();

	bool IsStackActive() const { return bIsActive; }

	void PushCameraMode(TSubclassOf<ULochCameraMode> CameraModeClass);

	bool EvaluateStack(float DeltaTime, FLochCameraModeView& OutCameraModeView);

	void DrawDebug(UCanvas* Canvas) const;

	// 获取与顶层相关联的标签及其混合权重
	void GetBlendInfo(float& OutWeightOfTopLayer, FGameplayTag& OutTagOfTopLayer) const;

protected:

	ULochCameraMode* GetCameraModeInstance(TSubclassOf<ULochCameraMode> CameraModeClass);

	void UpdateStack(float DeltaTime);
	void BlendStack(FLochCameraModeView& OutCameraModeView) const;

protected:

	bool bIsActive;

	UPROPERTY()
	TArray<TObjectPtr<ULochCameraMode>> CameraModeInstances;

	UPROPERTY()
	TArray<TObjectPtr<ULochCameraMode>> CameraModeStack;
};

#undef UE_API
