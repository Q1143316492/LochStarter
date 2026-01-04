#pragma once

#include "Engine/DataAsset.h"

#include "LochPawnData.generated.h"

#define UE_API LOCHSTARTERGAME_API

class APawn;
class UObject;


UCLASS(MinimalAPI, BlueprintType, Const, Meta = (DisplayName = "Loch Pawn Data", ShortTooltip = "Data asset used to define a Pawn."))
class ULochPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UE_API ULochPawnData(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loch|Pawn")
	TSubclassOf<APawn> PawnClass;
};