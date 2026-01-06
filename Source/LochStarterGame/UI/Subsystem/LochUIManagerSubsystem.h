// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Containers/Ticker.h"
#include "GameUIManagerSubsystem.h"

#include "LochUIManagerSubsystem.generated.h"

class FSubsystemCollectionBase;
class UObject;

UCLASS()
class ULochUIManagerSubsystem : public UGameUIManagerSubsystem
{
	GENERATED_BODY()

public:

	ULochUIManagerSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	bool Tick(float DeltaTime);
	void SyncRootLayoutVisibilityToShowHUD();
	
	FTSTicker::FDelegateHandle TickHandle;
};
