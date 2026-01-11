#pragma once

#include "Engine/DataAsset.h"

#include "LochPawnData.generated.h"

#define UE_API LOCHSTARTERGAME_API

class APawn;
class ULochAbilitySet;
class ULochAbilityTagRelationshipMapping;
class ULochCameraMode;
class ULochInputConfig;
class UObject;


UCLASS(MinimalAPI, BlueprintType, Const, Meta = (DisplayName = "Loch Pawn Data", ShortTooltip = "Data asset used to define a Pawn."))
class ULochPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UE_API ULochPawnData(const FObjectInitializer& ObjectInitializer);

public:
	// Class to instantiate for this pawn (should usually derive from ALochPawn or ALochCharacter).
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loch|Pawn")
	TSubclassOf<APawn> PawnClass;

	// Ability sets to grant to this pawn's ability system.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loch|Abilities")
	TArray<TObjectPtr<ULochAbilitySet>> AbilitySets;

	// What mapping of ability tags to use for actions taking by this pawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loch|Abilities")
	TObjectPtr<ULochAbilityTagRelationshipMapping> TagRelationshipMapping;

	// Input configuration used by player controlled pawns to create input mappings and bind input actions.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loch|Input")
	TObjectPtr<ULochInputConfig> InputConfig;

	// Default camera mode used by player controlled pawns.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loch|Camera")
	TSubclassOf<ULochCameraMode> DefaultCameraMode;
};

#undef UE_API