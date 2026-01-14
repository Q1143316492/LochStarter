// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Templates/SubclassOf.h"

#include "LochDevelopmentStatics.generated.h"

class UClass;
class UObject;
class UWorld;
struct FAssetData;
struct FFrame;

UCLASS()
class ULochDevelopmentStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Should game logic skip directly to gameplay (skipping any match warmup / waiting for players / etc... aspects)
	// 游戏逻辑是否应直接跳转到游戏玩法（跳过任何比赛热身/等待玩家等环节）
	// Will always return false except when playing in the editor and bTestFullGameFlowInPIE (in Loch Developer Settings) is false
	// 除非在编辑器中并且“Loch Developer Settings”中的 bTestFullGameFlowInPIE 为 false，否则总是返回 false
	UFUNCTION(BlueprintCallable, Category="Loch")
	static bool ShouldSkipDirectlyToGameplay();

	 // Should game logic load cosmetic backgrounds in the editor?
	 // 游戏逻辑在编辑器中是否应加载装饰性背景？
	 // Will always return true except when playing in the editor and bSkipLoadingCosmeticBackgroundsInPIE (in Loch Developer Settings) is true
	 // 除非在编辑器中并且“Loch Developer Settings”中的 bSkipLoadingCosmeticBackgroundsInPIE 为 true，否则总是返回 true
	UFUNCTION(BlueprintCallable, Category = "Loch", meta=(ExpandBoolAsExecs="ReturnValue"))
	static bool ShouldLoadCosmeticBackgrounds();

	 // Should game logic load cosmetic backgrounds in the editor?
	 // 游戏逻辑在编辑器中是否应加载装饰性背景？
	 // Will always return true except when playing in the editor and bSkipLoadingCosmeticBackgroundsInPIE (in Loch Developer Settings) is true
	 // 除非在编辑器中并且“Loch Developer Settings”中的 bSkipLoadingCosmeticBackgroundsInPIE 为 true，否则总是返回 true
	UFUNCTION(BlueprintCallable, Category = "Loch")
	static bool CanPlayerBotsAttack();

	 // Finds the most appropriate play-in-editor world to run 'server' cheats on
	 // 查找最合适的“在编辑器中运行”世界以执行“服务器”作弊命令
	 //   This might be the only world if running standalone, the listen server, or the dedicated server
	 //   如果是单机运行、监听服务器或专用服务器，这可能是唯一的世界
	static UWorld* FindPlayInEditorAuthorityWorld();

	 // Tries to find a class by a short name (with some heuristics to improve the usability when done via a cheat console)
	 // 尝试通过短名称查找类（带有一些启发式方法以提升通过作弊控制台使用时的易用性）
	static UClass* FindClassByShortName(const FString& SearchToken, UClass* DesiredBaseClass, bool bLogFailures = true);

	template <typename DesiredClass>
	static TSubclassOf<DesiredClass> FindClassByShortName(const FString& SearchToken, bool bLogFailures = true)
	{
		return FindClassByShortName(SearchToken, DesiredClass::StaticClass(), bLogFailures);
	}

private:
	static TArray<FAssetData> GetAllBlueprints();
	static UClass* FindBlueprintClass(const FString& TargetNameRaw, UClass* DesiredBaseClass);
};
