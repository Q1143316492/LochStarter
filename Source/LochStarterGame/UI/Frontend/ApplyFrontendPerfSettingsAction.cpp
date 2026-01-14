// Copyright Epic Games, Inc. All Rights Reserved.

#include "ApplyFrontendPerfSettingsAction.h"

#include "Settings/LochSettingsLocal.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ApplyFrontendPerfSettingsAction)

struct FGameFeatureActivatingContext;
struct FGameFeatureDeactivatingContext;

//////////////////////////////////////////////////////////////////////
// UApplyFrontendPerfSettingsAction

// Game user settings (and engine performance/scalability settings they drive)
// are global, so there's no point in tracking this per world for multi-player PIE:
// we just apply it if any PIE world is in the menu.
//
// However, by default we won't apply front-end performance stuff in the editor
// unless the developer setting ApplyFrontEndPerformanceOptionsInPIE is enabled
// 游戏用户设置（以及它们所驱动的引擎性能 / 可扩展性设置）
// 是全局的，因此在多人游戏 PIE 中按世界跟踪这些设置没有意义：
// 只要有任何 PIE 世界处于菜单状态，我们就应用这些设置。
//
// 不过，默认情况下，除非启用了开发者设置 ApplyFrontEndPerformanceOptionsInPIE，
// 否则我们不会在编辑器中应用前端性能相关内容

int32 UApplyFrontendPerfSettingsAction::ApplicationCounter = 0;

void UApplyFrontendPerfSettingsAction::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	ApplicationCounter++;
	if (ApplicationCounter == 1)
	{
		ULochSettingsLocal::Get()->SetShouldUseFrontendPerformanceSettings(true);
	}
}

void UApplyFrontendPerfSettingsAction::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	ApplicationCounter--;
	check(ApplicationCounter >= 0);

	if (ApplicationCounter == 0)
	{
		ULochSettingsLocal::Get()->SetShouldUseFrontendPerformanceSettings(false);
	}
}

