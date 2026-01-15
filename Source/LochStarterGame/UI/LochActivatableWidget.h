// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CommonActivatableWidget.h"

#include "LochActivatableWidget.generated.h"

struct FUIInputConfig;

UENUM(BlueprintType)
enum class ELochWidgetInputMode : uint8
{
	Default,
	GameAndMenu,
	Game,
	Menu
};

// An activatable widget that automatically drives the desired input config when activated
UCLASS(Abstract, Blueprintable)
class ULochActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	ULochActivatableWidget(const FObjectInitializer& ObjectInitializer);
	
public:
	
	//~UCommonActivatableWidget interface
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;
	//~End of UCommonActivatableWidget interface

#if WITH_EDITOR
	virtual void ValidateCompiledWidgetTree(const UWidgetTree& BlueprintWidgetTree, class IWidgetCompilerLog& CompileLog) const override;
#endif
	
protected:
	/** The desired input mode to use while this UI is activated, for example do you want key presses to still reach the game/player controller? */
	// 用户界面激活时所需的输入模式，例如您是否希望按键仍然到达游戏/玩家控制器？
	UPROPERTY(EditDefaultsOnly, Category = Input)
	ELochWidgetInputMode InputConfig = ELochWidgetInputMode::Default;

	/** The desired mouse behavior when the game gets input. */
	// 游戏获取输入时所需的鼠标行为。
	UPROPERTY(EditDefaultsOnly, Category = Input)
	EMouseCaptureMode GameMouseCaptureMode = EMouseCaptureMode::CapturePermanently;
};
