#include "LochGameMode.h"
#include "LochGameState.h"
#include "Player/LochPlayerState.h"
#include "Player/LochPlayerController.h"
#include "System/LochGameSession.h"
#include "Character/LochCharacter.h"
#include "Character/LochPawnData.h"
#include "UI/LochHUD.h"
#include "GameModes/LochWorldSettings.h"
#include "GameModes/LochExperienceDefinition.h"
#include "GameModes/LochUserFacingExperienceDefinition.h"
#include "GameModes/LochExperienceManagerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/CommandLine.h"
#include "Engine/AssetManager.h"
#include "GameMapsSettings.h"
#include "CommonUserSubsystem.h"
#include "CommonSessionSubsystem.h"
#include "LochLogChannels.h"
#include "TimerManager.h"
#include "Player/LochPlayerSpawningManagerComponent.h"


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

const ULochPawnData* ALochGameMode::GetPawnDataForController(const AController* InController) const
{
	// See if pawn data is already set on the player state
	if (InController != nullptr)
	{
		if (const ALochPlayerState* LochPS = InController->GetPlayerState<ALochPlayerState>())
		{
			if (const ULochPawnData* PawnData = LochPS->GetPawnData<ULochPawnData>())
			{
				return PawnData;
			}
		}
	}

	check(GameState);
	ULochExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<ULochExperienceManagerComponent>();
	check(ExperienceComponent);

	if (ExperienceComponent->IsExperienceLoaded())
	{
		const ULochExperienceDefinition* Experience = ExperienceComponent->GetCurrentExperienceChecked();
		if (Experience->DefaultPawnData != nullptr)
		{
			return Experience->DefaultPawnData;
		}

		// Experience is loaded and there's still no pawn data, fall back to the default for now
		return nullptr;
	}

	// Experience not loaded yet, so there is no pawn data to be had
	return nullptr;
}

void ALochGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);
}

void ALochGameMode::HandleMatchAssignmentIfNotExpectingOne()
{
	FPrimaryAssetId ExperienceId;
	FString ExperienceIdSource;

	// 优先级顺序 (最高优先级胜出)
	//  - 匹配分配 (如果存在)
	//  - URL 选项覆盖 (URL Options override)
	//  - 开发者设置 (仅 PIE 模式)
	//  - 命令行覆盖 (Command Line override)
	//  - 世界设置 (World Settings)
	//  - 专用服务器 (Dedicated server)
	//  - 默认体验 (Default experience)

	UWorld* World = GetWorld();

	// 1. 检查 URL 参数中是否有 "Experience" 选项 (例如: ?Experience=B_MyExperience)
	if (!ExperienceId.IsValid() && UGameplayStatics::HasOption(OptionsString, TEXT("Experience")))
	{
		const FString ExperienceFromOptions = UGameplayStatics::ParseOption(OptionsString, TEXT("Experience"));
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType(ULochExperienceDefinition::StaticClass()->GetFName()), FName(*ExperienceFromOptions));
		ExperienceIdSource = TEXT("OptionsString");
	}

	// 2. 检查命令行参数中是否有 "Experience=" 设置 (例如: -Experience=B_MyExperience)
	if (!ExperienceId.IsValid())
	{
		FString ExperienceFromCommandLine;
		if (FParse::Value(FCommandLine::Get(), TEXT("Experience="), ExperienceFromCommandLine))
		{
			ExperienceId = FPrimaryAssetId::ParseTypeAndName(ExperienceFromCommandLine);
			if (!ExperienceId.PrimaryAssetType.IsValid())
			{
				ExperienceId = FPrimaryAssetId(FPrimaryAssetType(ULochExperienceDefinition::StaticClass()->GetFName()), FName(*ExperienceFromCommandLine));
			}
			ExperienceIdSource = TEXT("CommandLine");
		}
	}

	// 3. 检查当前关卡的世界设置 (World Settings) 中是否配置了默认 Experience
	if (!ExperienceId.IsValid())
	{
		if (ALochWorldSettings* TypedWorldSettings = Cast<ALochWorldSettings>(GetWorldSettings()))
		{
			ExperienceId = TypedWorldSettings->GetDefaultGameplayExperience();
			ExperienceIdSource = TEXT("WorldSettings");
		}
	}

	// 验证找到的 Experience 是否有效 (是否存在于 AssetManager 中)
	UAssetManager& AssetManager = UAssetManager::Get();
	FAssetData Dummy;
	if (ExperienceId.IsValid() && !AssetManager.GetPrimaryAssetData(ExperienceId, /*out*/ Dummy))
	{
		UE_LOG(LogLochExperience, Error, TEXT("EXPERIENCE: Wanted to use %s but couldn't find it, falling back to the default)"), *ExperienceId.ToString());
		ExperienceId = FPrimaryAssetId();
	}

	// 4. 最终回退方案：使用默认 Experience
	if (!ExperienceId.IsValid())
	{
		// 如果是专用服务器，尝试登录并托管
		if (TryDedicatedServerLogin())
		{
			// 这将开始作为专用服务器托管
			return;
		}

		//@TODO: 从配置设置或其他地方获取此值
		// 如果没有找到任何 Experience，使用硬编码的默认值 "B_LochDefaultExperience"
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType("LochExperienceDefinition"), FName("B_LochDefaultExperience"));
		ExperienceIdSource = TEXT("Default");
	}

	// 确定了 Experience 后，通知 GameMode 进行处理 (加载并应用)
	OnMatchAssignmentGiven(ExperienceId, ExperienceIdSource);
}

void ALochGameMode::OnExperienceLoaded(const ULochExperienceDefinition* CurrentExperience)
{
	// Spawn any players that are already attached
	//@TODO: Here we're handling only *player* controllers, but in GetDefaultPawnClassForController_Implementation we skipped all controllers
	// GetDefaultPawnClassForController_Implementation might only be getting called for players anyways
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Cast<APlayerController>(*Iterator);
		if ((PC != nullptr) && (PC->GetPawn() == nullptr))
		{
			if (PlayerCanRestart(PC))
			{
				RestartPlayer(PC);
			}
		}
	}
}

bool ALochGameMode::IsExperienceLoaded() const
{
	check(GameState);
	ULochExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<ULochExperienceManagerComponent>();
	check(ExperienceComponent);

	return ExperienceComponent->IsExperienceLoaded();
}


void ALochGameMode::OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId, const FString& ExperienceIdSource)
{
	if (ExperienceId.IsValid())
	{
		UE_LOG(LogLochExperience, Log, TEXT("Identified experience %s (Source: %s)"), *ExperienceId.ToString(), *ExperienceIdSource);

		ULochExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<ULochExperienceManagerComponent>();
		check(ExperienceComponent);
		ExperienceComponent->SetCurrentExperience(ExperienceId);
	}
	else
	{
		UE_LOG(LogLochExperience, Error, TEXT("Failed to identify experience, loading screen will stay up forever"));
	}
}

bool ALochGameMode::TryDedicatedServerLogin()
{
	// Some basic code to register as an active dedicated server, this would be heavily modified by the game
	FString DefaultMap = UGameMapsSettings::GetGameDefaultMap();
	UWorld* World = GetWorld();
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance && World && World->GetNetMode() == NM_DedicatedServer && World->URL.Map == DefaultMap)
	{
		// Only register if this is the default map on a dedicated server
		UCommonUserSubsystem* UserSubsystem = GameInstance->GetSubsystem<UCommonUserSubsystem>();

		// Dedicated servers may need to do an online login
		UserSubsystem->OnUserInitializeComplete.AddDynamic(this, &ALochGameMode::OnUserInitializedForDedicatedServer);

		// There are no local users on dedicated server, but index 0 means the default platform user which is handled by the online login code
		if (!UserSubsystem->TryToLoginForOnlinePlay(0))
		{
			OnUserInitializedForDedicatedServer(nullptr, false, FText(), ECommonUserPrivilege::CanPlayOnline, ECommonUserOnlineContext::Default);
		}

		return true;
	}

	return false;
}

void ALochGameMode::HostDedicatedServerMatch(ECommonSessionOnlineMode OnlineMode)
{
	FPrimaryAssetType UserExperienceType = ULochUserFacingExperienceDefinition::StaticClass()->GetFName();
	
	// Figure out what UserFacingExperience to load
	FPrimaryAssetId UserExperienceId;
	FString UserExperienceFromCommandLine;
	if (FParse::Value(FCommandLine::Get(), TEXT("UserExperience="), UserExperienceFromCommandLine) ||
		FParse::Value(FCommandLine::Get(), TEXT("Playlist="), UserExperienceFromCommandLine))
	{
		UserExperienceId = FPrimaryAssetId::ParseTypeAndName(UserExperienceFromCommandLine);
		if (!UserExperienceId.PrimaryAssetType.IsValid())
		{
			UserExperienceId = FPrimaryAssetId(FPrimaryAssetType(UserExperienceType), FName(*UserExperienceFromCommandLine));
		}
	}

	// Search for the matching experience, it's fine to force load them because we're in dedicated server startup
	UAssetManager& AssetManager = UAssetManager::Get();
	TSharedPtr<FStreamableHandle> Handle = AssetManager.LoadPrimaryAssetsWithType(UserExperienceType);
	if (ensure(Handle.IsValid()))
	{
		Handle->WaitUntilComplete();
	}

	TArray<UObject*> UserExperiences;
	AssetManager.GetPrimaryAssetObjectList(UserExperienceType, UserExperiences);
	ULochUserFacingExperienceDefinition* FoundExperience = nullptr;
	ULochUserFacingExperienceDefinition* DefaultExperience = nullptr;

	for (UObject* Object : UserExperiences)
	{
		ULochUserFacingExperienceDefinition* UserExperience = Cast<ULochUserFacingExperienceDefinition>(Object);
		if (ensure(UserExperience))
		{
			if (UserExperience->GetPrimaryAssetId() == UserExperienceId)
			{
				FoundExperience = UserExperience;
				break;
			}
			
			if (UserExperience->bIsDefaultExperience && DefaultExperience == nullptr)
			{
				DefaultExperience = UserExperience;
			}
		}
	}

	if (FoundExperience == nullptr)
	{
		FoundExperience = DefaultExperience;
	}
	
	UGameInstance* GameInstance = GetGameInstance();
	if (ensure(FoundExperience && GameInstance))
	{
		// Actually host the game
		UCommonSession_HostSessionRequest* HostRequest = FoundExperience->CreateHostingRequest(this);
		if (ensure(HostRequest))
		{
			HostRequest->OnlineMode = OnlineMode;

			// TODO override other parameters?

			UCommonSessionSubsystem* SessionSubsystem = GameInstance->GetSubsystem<UCommonSessionSubsystem>();
			SessionSubsystem->HostSession(nullptr, HostRequest);
			
			// This will handle the map travel
		}
	}

}

void ALochGameMode::OnUserInitializedForDedicatedServer(const UCommonUserInfo* UserInfo, bool bSuccess, FText Error, ECommonUserPrivilege RequestedPrivilege, ECommonUserOnlineContext OnlineContext)
{
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UCommonUserSubsystem* UserSubsystem = GameInstance->GetSubsystem<UCommonUserSubsystem>();
		UserSubsystem->OnUserInitializeComplete.RemoveDynamic(this, &ALochGameMode::OnUserInitializedForDedicatedServer);

		if (bSuccess)
		{
			UE_LOG(LogLochExperience, Log, TEXT("Dedicated server user initialized successfully"));
		}
		else
		{
			UE_LOG(LogLochExperience, Error, TEXT("Dedicated server user initialization failed: %s"), *Error.ToString());
		}

		HostDedicatedServerMatch(ECommonSessionOnlineMode::Online);
	}
}

UClass* ALochGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (const ULochPawnData* PawnData = GetPawnDataForController(InController))
	{
		if (PawnData->PawnClass)
		{
			return PawnData->PawnClass;
		}
	}

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

APawn* ALochGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;	// Never save the default player pawns into a map.
	SpawnInfo.bDeferConstruction = true;

	if (UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if (APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo))
		{
			// TODO cwl 这个类还没有
			// if (ULochPawnExtensionComponent* PawnExtComp = ULochPawnExtensionComponent::FindPawnExtensionComponent(SpawnedPawn))
			// {
			// 	if (const ULochPawnData* PawnData = GetPawnDataForController(NewPlayer))
			// 	{
			// 		PawnExtComp->SetPawnData(PawnData);
			// 	}
			// 	else
			// 	{
			// 		UE_LOG(LogLoch, Error, TEXT("Game mode was unable to set PawnData on the spawned pawn [%s]."), *GetNameSafe(SpawnedPawn));
			// 	}
			// }

			SpawnedPawn->FinishSpawning(SpawnTransform);

			return SpawnedPawn;
		}
		else
		{
			UE_LOG(LogLoch, Error, TEXT("Game mode was unable to spawn Pawn of class [%s] at [%s]."), *GetNameSafe(PawnClass), *SpawnTransform.ToHumanReadableString());
		}
	}
	else
	{
		UE_LOG(LogLoch, Error, TEXT("Game mode was unable to spawn Pawn due to NULL pawn class."));
	}

	return nullptr;
}

bool ALochGameMode::ShouldSpawnAtStartSpot(AController* Player)
{
	// We never want to use the start spot, always use the spawn management component.
	return false;
}

void ALochGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	// Delay starting new players until the experience has been loaded
	// (players who log in prior to that will be started by OnExperienceLoaded)
	if (IsExperienceLoaded())
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

AActor* ALochGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	if (ULochPlayerSpawningManagerComponent* PlayerSpawningComponent = GameState->FindComponentByClass<ULochPlayerSpawningManagerComponent>())
	{
		return PlayerSpawningComponent->ChoosePlayerStart(Player);
	}
	
	return Super::ChoosePlayerStart_Implementation(Player);
}

void ALochGameMode::FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation)
{
	if (ULochPlayerSpawningManagerComponent* PlayerSpawningComponent = GameState->FindComponentByClass<ULochPlayerSpawningManagerComponent>())
	{
		PlayerSpawningComponent->FinishRestartPlayer(NewPlayer, StartRotation);
	}

	Super::FinishRestartPlayer(NewPlayer, StartRotation);
}

bool ALochGameMode::PlayerCanRestart_Implementation(APlayerController* Player)
{
	return ControllerCanRestart(Player);
}

bool ALochGameMode::ControllerCanRestart(AController* Controller)
{
	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{	
		if (!Super::PlayerCanRestart_Implementation(PC))
		{
			return false;
		}
	}
	else
	{
		// Bot version of Super::PlayerCanRestart_Implementation
		if ((Controller == nullptr) || Controller->IsPendingKillPending())
		{
			return false;
		}
	}

	if (ULochPlayerSpawningManagerComponent* PlayerSpawningComponent = GameState->FindComponentByClass<ULochPlayerSpawningManagerComponent>())
	{
		return PlayerSpawningComponent->ControllerCanRestart(Controller);
	}

	return true;
}

void ALochGameMode::InitGameState()
{
	Super::InitGameState();
	ULochExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<ULochExperienceManagerComponent>();
	check(ExperienceComponent);
	ExperienceComponent->CallOrRegister_OnExperienceLoaded(FOnLochExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

bool ALochGameMode::UpdatePlayerStartSpot(AController* Player, const FString& Portal, FString& OutErrorMessage)
{
	// Do nothing, we'll wait until PostLogin when we try to spawn the player for real.
	// Doing anything right now is no good, systems like team assignment haven't even occurred yet.
	return true;
}

void ALochGameMode::GenericPlayerInitialization(AController* NewPlayer)
{
	Super::GenericPlayerInitialization(NewPlayer);

	OnGameModePlayerInitialized.Broadcast(this, NewPlayer);
}

void ALochGameMode::FailedToRestartPlayer(AController* NewPlayer)
{
	Super::FailedToRestartPlayer(NewPlayer);
	// TODO: Implement
}