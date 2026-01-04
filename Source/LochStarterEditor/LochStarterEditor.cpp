// Copyright Epic Games, Inc. All Rights Reserved.

#include "LochStarterEditor.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "LochStarterEditor"

DEFINE_LOG_CATEGORY(LogLochStarterEditor);


/**
 * FLochStarterEditorModule
 */
class FLochStarterEditorModule : public FDefaultGameModuleImpl
{
	virtual void StartupModule() override
	{
	}

	virtual void ShutdownModule() override
	{
	}
};


IMPLEMENT_MODULE(FLochStarterEditorModule, LochStarterEditor);

#undef LOCTEXT_NAMESPACE