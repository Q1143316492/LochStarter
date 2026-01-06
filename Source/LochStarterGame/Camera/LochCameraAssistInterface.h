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
	 * Get the list of actors that we're allowing the camera to penetrate. Useful in 3rd person cameras
	 * when you need the following camera to ignore things like the a collection of view targets, the pawn,
	 * a vehicle..etc.
     * 获取我们允许相机穿透的演员列表。这在第三人称相机中很有用，当你需要跟随相机忽略诸如一系列视图目标、角色、车辆等物体时。
	 */
	virtual void GetIgnoredActorsForCameraPentration(TArray<const AActor*>& OutActorsAllowPenetration) const { }

	/**
	 * The target actor to prevent penetration on.  Normally, this is almost always the view target, which if
	 * unimplemented will remain true.  However, sometimes the view target, isn't the same as the root actor 
	 * you need to keep in frame.
     * 防止穿透的目标演员。通常，这几乎总是视图目标，如果未实现则保持不变。然而，有时视图目标并不等同于你需要保持在画面中的根演员。
	 */
	virtual TOptional<AActor*> GetCameraPreventPenetrationTarget() const
	{
		return TOptional<AActor*>();
	}

	/** Called if the camera penetrates the focal target.  Useful if you want to hide the target actor when being overlapped. */
	virtual void OnCameraPenetratingTarget() { }
};
