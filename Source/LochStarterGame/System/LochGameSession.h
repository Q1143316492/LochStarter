#pragma once

#include "GameFramework/GameSession.h"
#include "LochGameSession.generated.h"

class UObject;


UCLASS(Config = Game)
class ALochGameSession : public AGameSession
{
	GENERATED_BODY()

public:

	ALochGameSession(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	virtual bool ProcessAutoLogin() override;

	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;
};
