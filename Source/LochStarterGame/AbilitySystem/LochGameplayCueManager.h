#pragma once

#include "GameplayCueManager.h"

#include "LochGameplayCueManager.generated.h"

class FString;
class UClass;
class UObject;
class UWorld;
struct FObjectKey;

UCLASS()
class ULochGameplayCueManager : public UGameplayCueManager
{
	GENERATED_BODY()

public:
	ULochGameplayCueManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static ULochGameplayCueManager* Get();

    void LoadAlwaysLoadedCues();
};