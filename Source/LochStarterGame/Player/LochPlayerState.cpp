#include "LochPlayerState.h"
#include "AbilitySystem/LochAbilitySystemComponent.h"
#include "Player/LochPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Character/LochPawnData.h"
#include "LochLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LochPlayerState)


ALochPlayerState::ALochPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<ULochAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    // AbilitySystemComponent needs to be updated at a high frequency.
	SetNetUpdateFrequency(100.0f);
}

ALochPlayerController* ALochPlayerState::GetLochPlayerController() const
{
    return Cast<ALochPlayerController>(GetOwner());
}

UAbilitySystemComponent* ALochPlayerState::GetAbilitySystemComponent() const
{
	return GetLochAbilitySystemComponent();
}

void ALochPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALochPlayerState, PawnData);
}

void ALochPlayerState::OnRep_PawnData()
{
}

void ALochPlayerState::SetPawnData(const ULochPawnData* InPawnData)
{
	check(InPawnData);

	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		UE_LOG(LogLoch, Error, TEXT("Trying to set PawnData [%s] on player state [%s] that already has valid PawnData [%s]."), *GetNameSafe(InPawnData), *GetNameSafe(this), *GetNameSafe(PawnData));
		return;
	}

	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, PawnData, this);
	PawnData = InPawnData;

	// TODO skill and event
	
	ForceNetUpdate();
}