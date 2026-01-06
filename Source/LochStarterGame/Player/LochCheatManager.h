#pragma once

#include "GameFramework/CheatManager.h"
#include "LochCheatManager.generated.h"

class ULochAbilitySystemComponent;

#ifndef USING_CHEAT_MANAGER
#define USING_CHEAT_MANAGER (1 && !UE_BUILD_SHIPPING)
#endif // #ifndef USING_CHEAT_MANAGER

DECLARE_LOG_CATEGORY_EXTERN(LogLochCheat, Log, All);

UCLASS(config = Game, Within = PlayerController, MinimalAPI)
class ULochCheatManager : public UCheatManager
{
	GENERATED_BODY()

public:

	ULochCheatManager();

	virtual void InitCheatManager() override;

	// Helper function to write text to the console and to the log.
	static void CheatOutputText(const FString& TextToOutput);

	// Prevents the owning player from taking any damage.
	virtual void God() override;

};