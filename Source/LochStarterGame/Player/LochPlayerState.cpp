#include "LochPlayerState.h"
#include "AbilitySystem/LochAbilitySystemComponent.h"
#include "Player/LochPlayerController.h"
#include "Net/UnrealNetwork.h"

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