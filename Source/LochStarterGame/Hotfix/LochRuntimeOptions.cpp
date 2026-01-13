// Copyright Epic Games, Inc. All Rights Reserved.

#include "LochRuntimeOptions.h"

#include "UObject/Class.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LochRuntimeOptions)

ULochRuntimeOptions::ULochRuntimeOptions()
{
	OptionCommandPrefix = TEXT("ro");
}

ULochRuntimeOptions* ULochRuntimeOptions::GetRuntimeOptions()
{
	return GetMutableDefault<ULochRuntimeOptions>();
}

const ULochRuntimeOptions& ULochRuntimeOptions::Get()
{
	const ULochRuntimeOptions& RuntimeOptions = *GetDefault<ULochRuntimeOptions>();
	return RuntimeOptions;
}
