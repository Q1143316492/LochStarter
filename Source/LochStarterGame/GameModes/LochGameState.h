#pragma once

#include "AbilitySystemInterface.h"
#include "ModularGameState.h"

#include "LochGameState.generated.h"

#define UE_API LOCHSTARTERGAME_API


struct FLyraVerbMessage;

class APlayerState;
class UAbilitySystemComponent;
class ULochAbilitySystemComponent;
class ULochExperienceManagerComponent;
class UObject;
struct FFrame;

/**
 * ALochGameState
 *
 *	The base game state class used by this project.
 */
UCLASS(MinimalAPI, Config = Game)
class ALochGameState : public AModularGameStateBase, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	UE_API ALochGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~IAbilitySystemInterface
	UE_API virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~End of IAbilitySystemInterface

private:
	UPROPERTY()
	TObjectPtr<ULochExperienceManagerComponent> ExperienceManagerComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Loch|GameState")
	TObjectPtr<ULochAbilitySystemComponent> AbilitySystemComponent;

};

#undef UE_API