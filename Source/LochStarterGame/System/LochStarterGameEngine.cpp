#include "LochStarterGameEngine.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LochStarterGameEngine)

class IEngineLoop;


ULochStarterGameEngine::ULochStarterGameEngine(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ULochStarterGameEngine::Init(IEngineLoop* InEngineLoop)
{
	Super::Init(InEngineLoop);
}

