#include "LochGameMode.h"
#include "LochGameState.h"
#include "Player/LochPlayerState.h"
#include "Player/LochPlayerController.h"
#include "System/LochGameSession.h"
#include "Character/LochCharacter.h"
#include "UI/LochHUD.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(LochGameMode)

ALochGameMode::ALochGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
    GameStateClass = ALochGameState::StaticClass();
    GameSessionClass = ALochGameSession::StaticClass();
    PlayerControllerClass = ALochPlayerController::StaticClass();
	ReplaySpectatorPlayerControllerClass = ALochReplayPlayerController::StaticClass();
	PlayerStateClass = ALochPlayerState::StaticClass();
	DefaultPawnClass = ALochCharacter::StaticClass();
	HUDClass = ALochHUD::StaticClass();
}