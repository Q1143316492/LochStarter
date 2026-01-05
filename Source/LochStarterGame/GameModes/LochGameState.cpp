#include "LochGameState.h"

#include "AbilitySystem/LochAbilitySystemComponent.h"
#include "Async/TaskGraphInterfaces.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameModes/LochExperienceManagerComponent.h"
// #include "Messages/LochVerbMessage.h"
#include "Player/LochPlayerState.h"
#include "LochLogChannels.h"
#include "Net/UnrealNetwork.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(LochGameState)

class APlayerState;
class FLifetimeProperty;

extern ENGINE_API float GAverageFPS;

ALochGameState::ALochGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<ULochAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	ExperienceManagerComponent = CreateDefaultSubobject<ULochExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));
}

UAbilitySystemComponent* ALochGameState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}