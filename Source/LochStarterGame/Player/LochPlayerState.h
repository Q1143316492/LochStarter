#pragma once

#include "AbilitySystemInterface.h"
#include "ModularPlayerState.h"

#include "LochPlayerState.generated.h"

#define UE_API LOCHSTARTERGAME_API


class ULochPawnData;
/**
 * ALochPlayerState
 *
 *	Base player state class used by this project.
 */
UCLASS(MinimalAPI, Config = Game)
class ALochPlayerState : public AModularPlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	UE_API ALochPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintCallable, Category = "Loch|PlayerState")
	UE_API ALochPlayerController* GetLochPlayerController() const;

	UFUNCTION(BlueprintCallable, Category = "Loch|PlayerState")
	ULochAbilitySystemComponent* GetLochAbilitySystemComponent() const { return AbilitySystemComponent; }
	UE_API virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	//~UObject interface
	UE_API virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//~End of UObject interface

protected:
	UFUNCTION()
	UE_API void OnRep_PawnData();

protected:
    UPROPERTY(ReplicatedUsing = OnRep_PawnData)
	TObjectPtr<const ULochPawnData> PawnData;

private:
	UPROPERTY(VisibleAnywhere, Category = "Loch|PlayerState")
	TObjectPtr<ULochAbilitySystemComponent> AbilitySystemComponent;
};

#undef UE_API