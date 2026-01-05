#include "LochPlayerController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LochPlayerController)

namespace Loch
{
	namespace Input
	{
		static int32 ShouldAlwaysPlayForceFeedback = 0;
		static FAutoConsoleVariableRef CVarShouldAlwaysPlayForceFeedback(TEXT("LochPC.ShouldAlwaysPlayForceFeedback"),
			ShouldAlwaysPlayForceFeedback,
			TEXT("Should force feedback effects be played, even if the last input device was not a gamepad?"));
	}
}

ALochPlayerController::ALochPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
    // PlayerCameraManagerClass = ALochPlayerCameraManager::StaticClass();
}