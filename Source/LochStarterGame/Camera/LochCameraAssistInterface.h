// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "LochCameraAssistInterface.generated.h"

/** */
UINTERFACE(BlueprintType)
class ULochCameraAssistInterface : public UInterface
{
	GENERATED_BODY()
};

class ILochCameraAssistInterface
{
	GENERATED_BODY()

public:
	/**
     * 获取我们允许相机穿透的演员列表。这在第三人称相机中很有用，当你需要跟随相机忽略诸如一系列视图目标、角色、车辆等物体时。
	 */
	virtual void GetIgnoredActorsForCameraPentration(TArray<const AActor*>& OutActorsAllowPenetration) const { }

	/**
     * 防止穿透的目标演员。通常，这几乎总是视图目标，如果未实现则保持不变。然而，有时视图目标并不等同于你需要保持在画面中的根演员。
	 */
	virtual TOptional<AActor*> GetCameraPreventPenetrationTarget() const
	{
		return TOptional<AActor*>();
	}

	/** 
	 * 当相机穿透焦点目标时调用。如果您想在目标演员被重叠时隐藏它，这会很有用。
	 * */
	virtual void OnCameraPenetratingTarget() { }
};
