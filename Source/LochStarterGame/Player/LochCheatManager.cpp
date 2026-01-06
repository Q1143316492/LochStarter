#include "LochCheatManager.h"
#include "Development/LochDeveloperSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LochCheatManager)

DEFINE_LOG_CATEGORY(LogLochCheat);

namespace LochCheat
{
	static const FName NAME_Fixed = FName(TEXT("Fixed"));
	
	static bool bEnableDebugCameraCycling = false;
	static FAutoConsoleVariableRef CVarEnableDebugCameraCycling(
		TEXT("LochCheat.EnableDebugCameraCycling"),
		bEnableDebugCameraCycling,
		TEXT("If true then you can cycle the debug camera while running the game."),
		ECVF_Cheat);

	static bool bStartInGodMode = false;
	static FAutoConsoleVariableRef CVarStartInGodMode(
		TEXT("LochCheat.StartInGodMode"),
		bStartInGodMode,
		TEXT("If true then the God cheat will be applied on begin play"),
		ECVF_Cheat);
};

ULochCheatManager::ULochCheatManager()
{
	// DebugCameraControllerClass = ALochDebugCameraController::StaticClass();
}

void ULochCheatManager::InitCheatManager()
{
	Super::InitCheatManager();

#if WITH_EDITOR
	if (GIsEditor)
	{
		APlayerController* PC = GetOuterAPlayerController();
		for (const FLochCheatToRun& CheatRow : GetDefault<ULochDeveloperSettings>()->CheatsToRun)
		{
			if (CheatRow.Phase == ECheatExecutionTime::OnCheatManagerCreated)
			{
				PC->ConsoleCommand(CheatRow.Cheat, /*bWriteToLog=*/ true);
			}
		}
	}
#endif

	if (LochCheat::bStartInGodMode)
	{
		God();	
	}
}

void ULochCheatManager::God()
{
    
}
