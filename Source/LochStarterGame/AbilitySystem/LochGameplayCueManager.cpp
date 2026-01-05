#include "LochGameplayCueManager.h"
#include "Engine/AssetManager.h"
#include "LochLogChannels.h"
#include "GameplayCueSet.h"
#include "AbilitySystemGlobals.h"
#include "GameplayTagsManager.h"
#include "UObject/UObjectThreadContext.h"
#include "Async/Async.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LochGameplayCueManager)


ULochGameplayCueManager::ULochGameplayCueManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

ULochGameplayCueManager* ULochGameplayCueManager::Get()
{
	return Cast<ULochGameplayCueManager>(UAbilitySystemGlobals::Get().GetGameplayCueManager());
}

void ULochGameplayCueManager::LoadAlwaysLoadedCues()
{

}