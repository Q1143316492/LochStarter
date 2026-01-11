// Copyright Epic Games, Inc. All Rights Reserved.

#include "LochPlayerCameraManager.h"

#include "Async/TaskGraphInterfaces.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "LochCameraComponent.h"
#include "LochUICameraManagerComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LochPlayerCameraManager)

class FDebugDisplayInfo;

static FName UICameraComponentName(TEXT("UICamera"));

ALochPlayerCameraManager::ALochPlayerCameraManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultFOV = LOCH_CAMERA_DEFAULT_FOV;
	ViewPitchMin = LOCH_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = LOCH_CAMERA_DEFAULT_PITCH_MAX;

	UICamera = CreateDefaultSubobject<ULochUICameraManagerComponent>(UICameraComponentName);
}

ULochUICameraManagerComponent* ALochPlayerCameraManager::GetUICameraComponent() const
{
	return UICamera;
}

void ALochPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	// If the UI Camera is looking at something, let it have priority.
	if (UICamera->NeedsToUpdateViewTarget())
	{
		Super::UpdateViewTarget(OutVT, DeltaTime);
		UICamera->UpdateViewTarget(OutVT, DeltaTime);
		return;
	}

	Super::UpdateViewTarget(OutVT, DeltaTime);
}

void ALochPlayerCameraManager::DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& YL, float& YPos)
{
	check(Canvas);

	FDisplayDebugManager& DisplayDebugManager = Canvas->DisplayDebugManager;

	DisplayDebugManager.SetFont(GEngine->GetSmallFont());
	DisplayDebugManager.SetDrawColor(FColor::Yellow);
	DisplayDebugManager.DrawString(FString::Printf(TEXT("LochPlayerCameraManager: %s"), *GetNameSafe(this)));

	Super::DisplayDebug(Canvas, DebugDisplay, YL, YPos);

	const APawn* Pawn = (PCOwner ? PCOwner->GetPawn() : nullptr);

	if (const ULochCameraComponent* CameraComponent = ULochCameraComponent::FindCameraComponent(Pawn))
	{
		CameraComponent->DrawDebug(Canvas);
	}
}
