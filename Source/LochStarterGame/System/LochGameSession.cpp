#include "LochGameSession.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LochGameSession)


ALochGameSession::ALochGameSession(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool ALochGameSession::ProcessAutoLogin()
{
	// This is actually handled in LochGameMode::TryDedicatedServerLogin
	return true;
}

void ALochGameSession::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
}

void ALochGameSession::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
}

