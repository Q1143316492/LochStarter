#pragma once

#include "CoreMinimal.h"
#include "Engine/GameEngine.h"
#include "LochStarterGameEngine.generated.h"


class IEngineLoop;
class UObject;

UCLASS()
class LOCHSTARTERGAME_API ULochStarterGameEngine : public UGameEngine
{
    GENERATED_BODY()

public:

	ULochStarterGameEngine(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	virtual void Init(IEngineLoop* InEngineLoop) override;
};