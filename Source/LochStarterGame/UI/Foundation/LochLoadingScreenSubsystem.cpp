// Copyright Epic Games, Inc. All Rights Reserved.

#include "UI/Foundation/LochLoadingScreenSubsystem.h"

#include "Blueprint/UserWidget.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LochLoadingScreenSubsystem)

class UUserWidget;

//////////////////////////////////////////////////////////////////////
// ULochLoadingScreenSubsystem

ULochLoadingScreenSubsystem::ULochLoadingScreenSubsystem()
{
}

void ULochLoadingScreenSubsystem::SetLoadingScreenContentWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (LoadingScreenWidgetClass != NewWidgetClass)
	{
		LoadingScreenWidgetClass = NewWidgetClass;

		OnLoadingScreenWidgetChanged.Broadcast(LoadingScreenWidgetClass);
	}
}

TSubclassOf<UUserWidget> ULochLoadingScreenSubsystem::GetLoadingScreenContentWidget() const
{
	return LoadingScreenWidgetClass;
}

