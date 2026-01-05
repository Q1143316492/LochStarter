#pragma once

#include "ModularGameMode.h"
#include "LochGameMode.generated.h"

class AActor;
class AController;
class AGameModeBase;
class APawn;
class APlayerController;
class UClass;
class ULochExperienceDefinition;
class ULochPawnData;
class UObject;
struct FFrame;
struct FPrimaryAssetId;
enum class ECommonSessionOnlineMode : uint8;
class UCommonUserInfo;
enum class ECommonUserPrivilege : uint8;
enum class ECommonUserOnlineContext : uint8;


/**
 * ALochGameMode
 *
 *	The base game mode class used by this project.
 */
UCLASS(MinimalAPI, Config = Game, Meta = (ShortTooltip = "The base game mode class used by this project."))
class ALochGameMode : public AModularGameModeBase
{
	GENERATED_BODY()
public:

	LOCHSTARTERGAME_API ALochGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Loch|Pawn")
	LOCHSTARTERGAME_API const ULochPawnData* GetPawnDataForController(const AController* InController) const;

	//~AGameModeBase interface
	LOCHSTARTERGAME_API virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	LOCHSTARTERGAME_API virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	LOCHSTARTERGAME_API virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
	LOCHSTARTERGAME_API virtual bool ShouldSpawnAtStartSpot(AController* Player) override;
	LOCHSTARTERGAME_API virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	LOCHSTARTERGAME_API virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	LOCHSTARTERGAME_API virtual void FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation) override;
	LOCHSTARTERGAME_API virtual bool PlayerCanRestart_Implementation(APlayerController* Player) override;
	LOCHSTARTERGAME_API virtual void InitGameState() override;
	LOCHSTARTERGAME_API virtual bool UpdatePlayerStartSpot(AController* Player, const FString& Portal, FString& OutErrorMessage) override;
	LOCHSTARTERGAME_API virtual void GenericPlayerInitialization(AController* NewPlayer) override;
	LOCHSTARTERGAME_API virtual void FailedToRestartPlayer(AController* NewPlayer) override;
	//~End of AGameModeBase interface

protected:
	LOCHSTARTERGAME_API void OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId, const FString& ExperienceIdSource);

	LOCHSTARTERGAME_API void HandleMatchAssignmentIfNotExpectingOne();

	LOCHSTARTERGAME_API bool TryDedicatedServerLogin();
	LOCHSTARTERGAME_API void HostDedicatedServerMatch(ECommonSessionOnlineMode OnlineMode);

	UFUNCTION()
	LOCHSTARTERGAME_API void OnUserInitializedForDedicatedServer(const UCommonUserInfo* UserInfo, bool bSuccess, FText Error, ECommonUserPrivilege RequestedPrivilege, ECommonUserOnlineContext OnlineContext);
};