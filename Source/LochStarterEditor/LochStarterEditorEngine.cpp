#include "LochStarterEditorEngine.h"

#include "Engine/GameInstance.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Settings/ContentBrowserSettings.h"
#include "Settings/LevelEditorPlaySettings.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "GameModes/LochWorldSettings.h"
#include "Development/LochDeveloperSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LochStarterEditorEngine)

class IEngineLoop;

#define LOCTEXT_NAMESPACE "LochStarterEditor"

ULochStarterEditorEngine::ULochStarterEditorEngine(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ULochStarterEditorEngine::Init(IEngineLoop* InEngineLoop)
{
	Super::Init(InEngineLoop);
}

void ULochStarterEditorEngine::Start()
{
	Super::Start();
}

void ULochStarterEditorEngine::Tick(float DeltaSeconds, bool bIdleMode)
{
	Super::Tick(DeltaSeconds, bIdleMode);
	
	FirstTickSetup();
}

void ULochStarterEditorEngine::FirstTickSetup()
{
	if (bFirstTickSetup)
	{
		return;
	}

	bFirstTickSetup = true;
	GetMutableDefault<UContentBrowserSettings>()->SetDisplayPluginFolders(true);

}

FGameInstancePIEResult ULochStarterEditorEngine::PreCreatePIEInstances(const bool bAnyBlueprintErrors, const bool bStartInSpectatorMode, const float PIEStartTime, const bool bSupportsOnlinePIE, int32& InNumOnlinePIEInstances)
{
    // 如果world setting里设置了强制单机模式，就把play in editor的模式改成单机模式
	if (const ALochWorldSettings* LochWorldSettings = Cast<ALochWorldSettings>(EditorWorld->GetWorldSettings()))
	{
		if (LochWorldSettings->ForceStandaloneNetMode)
		{
			EPlayNetMode OutPlayNetMode;
			PlaySessionRequest->EditorPlaySettings->GetPlayNetMode(OutPlayNetMode);
			if (OutPlayNetMode != PIE_Standalone)
			{
				PlaySessionRequest->EditorPlaySettings->SetPlayNetMode(PIE_Standalone);

				FNotificationInfo Info(LOCTEXT("ForcingStandaloneForFrontend", "Forcing NetMode: Standalone for the Frontend"));
				Info.ExpireDuration = 2.0f;
				FSlateNotificationManager::Get().AddNotification(Info);
			}
		}
	}

	// @TODO: Should add delegates that a *non-editor* module could bind to for PIE start/stop instead of poking directly
	GetDefault<ULochDeveloperSettings>()->OnPlayInEditorStarted();
	// GetDefault<ULyraPlatformEmulationSettings>()->OnPlayInEditorStarted();

	FGameInstancePIEResult Result = Super::PreCreatePIEServerInstance(bAnyBlueprintErrors, bStartInSpectatorMode, PIEStartTime, bSupportsOnlinePIE, InNumOnlinePIEInstances);
	return Result;
}

#undef LOCTEXT_NAMESPACE
