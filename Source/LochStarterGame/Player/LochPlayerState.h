#pragma once

#include "AbilitySystemInterface.h"
#include "ModularPlayerState.h"
#include "System/GameplayTagStack.h"
#include "Teams/LochTeamAgentInterface.h"

#include "LochPlayerState.generated.h"

#define UE_API LOCHSTARTERGAME_API

struct FLochVerbMessage;

class AController;
class ALochPlayerController;
class APlayerState;
class FName;
class UAbilitySystemComponent;
class ULochAbilitySystemComponent;
class ULochExperienceDefinition;
class UObject;
struct FFrame;
struct FGameplayTag;
class ULochPawnData;

/** Defines the types of client connected */
UENUM()
enum class ELochPlayerConnectionType : uint8
{
	// An active player
	Player = 0,

	// Spectator connected to a running game
	LiveSpectator,

	// Spectating a demo recording offline
	ReplaySpectator,

	// A deactivated player (disconnected)
	InactivePlayer
};

/**
 * ALochPlayerState
 *
 *	Base player state class used by this project.
 */
UCLASS(MinimalAPI, Config = Game)
class ALochPlayerState : public AModularPlayerState, public IAbilitySystemInterface, public ILochTeamAgentInterface
{
	GENERATED_BODY()

public:
	UE_API ALochPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintCallable, Category = "Loch|PlayerState")
	UE_API ALochPlayerController* GetLochPlayerController() const;

	UFUNCTION(BlueprintCallable, Category = "Loch|PlayerState")
	ULochAbilitySystemComponent* GetLochAbilitySystemComponent() const { return AbilitySystemComponent; }
	UE_API virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

	UE_API void SetPawnData(const ULochPawnData* InPawnData);

	//~AActor interface
	UE_API virtual void PreInitializeComponents() override;
	UE_API virtual void PostInitializeComponents() override;
	//~End of AActor interface

	//~APlayerState interface
	UE_API virtual void Reset() override;
	UE_API virtual void ClientInitialize(AController* C) override;
	UE_API virtual void CopyProperties(APlayerState* PlayerState) override;
	UE_API virtual void OnDeactivated() override;
	UE_API virtual void OnReactivated() override;
	//~End of APlayerState interface

	//~ILyraTeamAgentInterface interface
	UE_API virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	UE_API virtual FGenericTeamId GetGenericTeamId() const override;
	UE_API virtual FOnLochTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	//~End of ILochTeamAgentInterface interface

	static UE_API const FName NAME_LochAbilityReady;

	UE_API void SetPlayerConnectionType(ELochPlayerConnectionType NewType);
	ELochPlayerConnectionType GetPlayerConnectionType() const { return MyPlayerConnectionType; }

	UFUNCTION(BlueprintCallable)
	int32 GetSquadId() const
	{
		return MySquadID; // 这个还没有什么用，队伍子ID
	}

	UFUNCTION(BlueprintCallable)
	int32 GetTeamId() const
	{
		return GenericTeamIdToInteger(MyTeamID);
	}

	UE_API void SetSquadID(int32 NewSquadID);

	// Adds a specified number of stacks to the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Teams)
	UE_API void AddStatTagStack(FGameplayTag Tag, int32 StackCount);

	// Removes a specified number of stacks from the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Teams)
	UE_API void RemoveStatTagStack(FGameplayTag Tag, int32 StackCount);

	// Returns the stack count of the specified tag (or 0 if the tag is not present)
	UFUNCTION(BlueprintCallable, Category=Teams)
	UE_API int32 GetStatTagStackCount(FGameplayTag Tag) const;

	// Returns true if there is at least one stack of the specified tag
	UFUNCTION(BlueprintCallable, Category=Teams)
	UE_API bool HasStatTag(FGameplayTag Tag) const;

	// Send a message to just this player
	// (use only for client notifications like accolades, quest toasts, etc... that can handle being occasionally lost)
	UFUNCTION(Client, Unreliable, BlueprintCallable, Category = "Loch|PlayerState")
	UE_API void ClientBroadcastMessage(const FLochVerbMessage Message);

	// 设置和获取一个观战用的角度
	UE_API FRotator GetReplicatedViewRotation() const;
	UE_API void SetReplicatedViewRotation(const FRotator& NewRotation);

	//~UObject interface
	UE_API virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//~End of UObject interface

protected:
	UFUNCTION()
	UE_API void OnRep_PawnData();

private:
	UFUNCTION()
	UE_API void OnRep_MyTeamID(FGenericTeamId OldTeamID);

	UFUNCTION()
	UE_API void OnRep_MySquadID();

	UE_API void OnExperienceLoaded(const ULochExperienceDefinition* CurrentExperience);

protected:
    UPROPERTY(ReplicatedUsing = OnRep_PawnData)
	TObjectPtr<const ULochPawnData> PawnData;

private:
	UPROPERTY(VisibleAnywhere, Category = "Loch|PlayerState")
	TObjectPtr<ULochAbilitySystemComponent> AbilitySystemComponent;

	// TODO cwl 两个 attribute set

	UPROPERTY(Replicated)
	ELochPlayerConnectionType MyPlayerConnectionType;

	UPROPERTY()
	FOnLochTeamIndexChangedDelegate OnTeamChangedDelegate;

	UPROPERTY(ReplicatedUsing=OnRep_MyTeamID)
	FGenericTeamId MyTeamID;

	UPROPERTY(ReplicatedUsing=OnRep_MySquadID)
	int32 MySquadID;

	UPROPERTY(Replicated)
	FGameplayTagStackContainer StatTags;

	UPROPERTY(Replicated)
	FRotator ReplicatedViewRotation;
};

#undef UE_API