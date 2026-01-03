// Copyright Epic Games, Inc. All Rights Reserved.

#include "LochStarterEditor.h"
#include "Modules/ModuleManager.h"

/**
 * LochStarterEditorModule
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