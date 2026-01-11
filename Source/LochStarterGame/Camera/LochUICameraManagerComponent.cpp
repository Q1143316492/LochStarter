// Copyright Epic Games, Inc. All Rights Reserved.

#include "LochUICameraManagerComponent.h"

#include "GameFramework/HUD.h"
#include "GameFramework/PlayerController.h"
#include "LochPlayerCameraManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LochUICameraManagerComponent)

class AActor;
class FDebugDisplayInfo;

ULochUICameraManagerComponent* ULochUICameraManagerComponent::GetComponent(APlayerController* PC)
{
	if (PC != nullptr)
	{
		if (ALochPlayerCameraManager* PCCamera = Cast<ALochPlayerCameraManager>(PC->PlayerCameraManager))
		{
			return PCCamera->GetUICameraComponent();
		}
	}

	return nullptr;
}

ULochUICameraManagerComponent::ULochUICameraManagerComponent()
{
	bWantsInitializeComponent = true;

	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		// Register "showdebug" hook.
		if (!IsRunningDedicatedServer())
		{
			AHUD::OnShowDebugInfo.AddUObject(this, &ThisClass::OnShowDebugInfo);
		}
	}
}

void ULochUICameraManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void ULochUICameraManagerComponent::SetViewTarget(AActor* InViewTarget, FViewTargetTransitionParams TransitionParams)
{
	TGuardValue<bool> UpdatingViewTargetGuard(bUpdatingViewTarget, true);

	ViewTarget = InViewTarget;
	CastChecked<ALochPlayerCameraManager>(GetOwner())->SetViewTarget(ViewTarget, TransitionParams);
}

bool ULochUICameraManagerComponent::NeedsToUpdateViewTarget() const
{
	return false;
}

void ULochUICameraManagerComponent::UpdateViewTarget(struct FTViewTarget& OutVT, float DeltaTime)
{
}

void ULochUICameraManagerComponent::OnShowDebugInfo(AHUD* HUD, UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo, float& YL, float& YPos)
{
}
