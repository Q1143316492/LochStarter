// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Logging/LogMacros.h"

class UObject;

LOCHSTARTERGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogLoch, Log, All);
LOCHSTARTERGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogLochExperience, Log, All);
LOCHSTARTERGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogLochAbilitySystem, Log, All);
LOCHSTARTERGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogLochTeams, Log, All);

LOCHSTARTERGAME_API FString GetClientServerContextString(UObject* ContextObject = nullptr);
