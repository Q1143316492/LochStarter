# 初始化项目

## 1. 概述

个人框架抄lyra的。

## 2. 项目初始化

UE 5.7 源码引擎，创建全空项目，跑通vscode和rider的断点运行，达到一个初始环境ok的状态。准备好gitignore

1. lyra插件目录搞过来

比较通用了，就把他当成引擎模块用吧。

偷了这几个 AsyncMixin,CommonGame,CommonLoadingScreen,CommonUser,GameplayMessageRouter,GameSettings,GameSubtitles,ModularGameplayActors,PocketWorlds,UIExtension
偷过来后也别忘了uproject加一下

- 核心架构与通信

    - GameplayMessageRouter: 就是事件系统
    - AsyncMixin 我需要加载A和B才能开始工作

-  UI 扩展与通用功能

    - UIExtension
    - CommonGame
    - CommonUser
    - CommonLoadingScreen
    - GameSettings

- 模块化游戏功能

    - GameFeatures 不用管，后面创建gamefeature的时候会有的
    - ModularGameplayActors

- 其他工具

    - PocketWorlds 用于在 UI 中展示 3D 角色或物品的独立小场景（例如角色自定义界面中的 3D 预览）
    - GameSubtitles 增强的字幕系统

我又加了这几个 GameFeatures，ModularGameplay,EnhancedInput,DataRegistry 其他不熟后面再看吧。

把target和build.cs写好
```cmd
│  LochStarterClient.Target.cs
│  LochStarterEditor.Target.cs
│  LochStarterGame.Target.cs
│  LochStarterServer.Target.cs
│
├─LochStarterEditor
│      LochStarterEditor.Build.cs
│      LochStarterEditor.cpp
│      LochStarterEditor.h
│
└─LochStarterGame
        LochStarterGame.Build.cs
        LochStarterGameModule.cpp
```
别忘记了.uproject
```json
{
    "Modules": [
        {
            "Name": "LochStarterGame",
            "Type": "Runtime",
            "LoadingPhase": "Default",
            "AdditionalDependencies": [
                "DeveloperSettings",
                "Engine"
            ]
        },
        {
            "Name": "LochStarterEditor",
            "Type": "Editor",
            "LoadingPhase": "Default"
        }
    ]
}
```

## 3. Build.cs 和 Target.cs 配置详解

整理了 `Source` 目录下关键的构建配置文件知识点。

### Target.cs (构建目标配置)

在 `LochStarterGame.Target.cs` 中定义了静态方法 `ApplySharedLochStarterTargetSettings`，供 Client/Editor/Server 等其他 Target 复用配置。

**通用配置 (Shared Settings):**
- **构建版本**: `BuildSettingsVersion.V5`，`IncludeOrderVersion.Latest` (使用最新引擎构建标准)。
- **编译警告**: `ShadowVariableWarningLevel = WarningLevel.Error` (变量遮蔽视为错误，提高代码质量)。
- **Shipping 环境**:
  - `bUseLoggingInShipping = true` (允许 Shipping 包输出日志)。
  - `bDisableUnverifiedCertificates = true` (Shipping 包强制验证 HTTPS 证书)。

**LochStarterEditor.Target.cs 特有配置:**
- **TObjectPtr 强制**:
  ```csharp
  if (!bBuildAllModules)
  {
      NativePointerMemberBehaviorOverride = PointerMemberBehavior.Disallow;
  }
  ```
  强制要求使用 `TObjectPtr<T>` 替代原生指针 `T*` 来管理 UObject，否则 UHT 会报错。这是 UE5 的推荐做法。
- **插件启用**: 启用了 `RemoteSession` 用于触摸屏开发调试。

### Build.cs (模块构建配置)

**LochStarterGame.Build.cs (Runtime 模块):**
- **基础依赖**: `Core`, `CoreUObject`, `Engine`, `InputCore`, `EnhancedInput`。
- **调试安全**:
  ```csharp
  PublicDefinitions.Add("SHIPPING_DRAW_DEBUG_ERROR=1");
  ```
  在 Test/Shipping 构建中使用 `DrawDebug` 函数会产生编译错误，防止调试代码泄露到发布版本。
- **RPC/HTTP 安全**:
  引入了 `ExternalRpcRegistry` 和 `HTTPServer`，但在 Shipping 构建中通过宏定义剥离功能，移除潜在的安全漏洞：
  ```csharp
  if (Target.Configuration == UnrealTargetConfiguration.Shipping)
  {
      PublicDefinitions.Add("WITH_RPC_REGISTRY=0");
      PublicDefinitions.Add("WITH_HTTPSERVER_LISTENERS=0");
  }
  ```

**LochStarterEditor.Build.cs (Editor 模块):**
- 同样剥离了 RPC 注册表功能，保持 Shipping 版本纯净。

保持编译OK。

## 4. 导入蓝图资源

我决定先把 Content/Character 扣过来。X

全部扣过来，怎么找不到 lyra anim only 那个包了。

虽然很多蓝图炸了，但是动画，模型，曲线都正常过来了。

## 5. 体验定义

Lyra的新概念，应该是world settings配置体验，体验里面配置pawn data。

pawn data 里面是技能，输入，摄像机。先顺着这个把game play 的几个核心类搞起来。

- 自定义 Engine 类 LochStarterEditorEngine，ULochStarterGameEngine 

    - 目前没实际作用。可以默认打开Plugins目录，如果是PIE且world settings设置了单机模式，就强制PIE单机模式。 应该是给那个炸弹人用的。但是现在对我们没用。简单扣过来。

    - DefaultEngine.ini [/Script/Engine.Engine] 里面配置，断点一下没问题提交一次

- 继承world settings 类，能配置experience，然后根据experience配置pawn data，pawn data。编译成功，然后我们总结一下干了啥

首先

Project Settings -> World Settings Class 修改为我们自定义的一个类。修改后DefaultEngine.ini 会自动添加一行

主要是扩展了一个 data asset

```cpp
UCLASS(MinimalAPI)
class ALochWorldSettings : public AWorldSettings
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category=GameMode)
	TSoftClassPtr<ULochExperienceDefinition> DefaultGameplayExperience;
};
```

创建插件，game feature content only。失败了。DefaultEngine.ini 里面会自动添加。lyra也有类似的配置

只不过这里我要改成源码绝对路径

```
[/Script/GameFeaturesEditor.GameFeaturesEditorSettings]
-PluginTemplates=(Path=(Path="../../Plugins/Runtime/GameFeatures/Templates/GameFeaturePluginContentOnly"),Label=NSLOCTEXT("[/Script/GameFeaturesEditor]", "E17DB7E94A2C0F5968A52E89959864C0", "Game Feature (Content Only)"),Description=NSLOCTEXT("[/Script/GameFeaturesEditor]", "CB5136B64AFAFBA5EDEA078FCAC7BCC9", "Create a new Game Feature Plugin."),DefaultGameFeatureDataClass=None,DefaultGameFeatureDataName="")
-PluginTemplates=(Path=(Path="../../Plugins/Runtime/GameFeatures/Templates/GameFeaturePluginWithCode"),Label=NSLOCTEXT("[/Script/GameFeaturesEditor]", "90D6FD2A455176CEC42DD79A23AEC3B0", "Game Feature (with C++)"),Description=NSLOCTEXT("[/Script/GameFeaturesEditor]", "2B00A0E7415C8CEAA6DA609A141237F0", "Create a new Game Feature Plugin with a minimal amount of code."),DefaultGameFeatureDataClass=None,DefaultGameFeatureDataName="")
+PluginTemplates=(Path=(Path="F:/UE_57Src/UE571/Engine/Plugins/Runtime/GameFeatures/Templates/GameFeaturePluginContentOnly"),Label=NSLOCTEXT("[/Script/GameFeaturesEditor]", "E17DB7E94A2C0F5968A52E89959864C0", "Game Feature (Content Only)"),Description=NSLOCTEXT("[/Script/GameFeaturesEditor]", "CB5136B64AFAFBA5EDEA078FCAC7BCC9", "Create a new Game Feature Plugin."),DefaultGameFeatureDataClass=None,DefaultGameFeatureDataName="")
+PluginTemplates=(Path=(Path="F:/UE_57Src/UE571/Engine/Plugins/Runtime/GameFeatures/Templates/GameFeaturePluginWithCode"),Label=NSLOCTEXT("[/Script/GameFeaturesEditor]", "90D6FD2A455176CEC42DD79A23AEC3B0", "Game Feature (with C++)"),Description=NSLOCTEXT("[/Script/GameFeaturesEditor]", "2B00A0E7415C8CEAA6DA609A141237F0", "Create a new Game Feature Plugin with a minimal amount of code."),DefaultGameFeatureDataClass=None,DefaultGameFeatureDataName="")
```

创建成功后 `Plugins\GameFeatures\LochGameCore` 就有东西了，并且content下有个data asset 配置

在里面创建蓝图 `Experiences\B_DefaultExperience` 继承自 `LochExperienceDefinition`

```cpp
/**
 * Definition of an experience
 */
UCLASS(BlueprintType, Const)
class LOCHSTARTERGAME_API ULochExperienceDefinition : public UPrimaryDataAsset
{
    GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TArray<FString> GameFeaturesToEnable;

	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TObjectPtr<const ULochPawnData> DefaultPawnData;

    UPROPERTY(EditDefaultsOnly, Instanced, Category="Actions")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;

    UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TArray<TObjectPtr<ULochExperienceActionSet>> ActionSets;
};
```

UGameFeatureAction 相当于加载了当前game feature的时候执行的回调。

例如 GameFeatureAction_AddComponents 就是一个Action，可以额外的添两个参数一个类和一个组件。在激活时就会给这个类动态加上这个组件。 

这里除了 PawnData 之外, 都是对于 GameFeatureAction 的包装。位于 LochGameCore/Game

pawn data 现在还什么都没有，后面写到了会加输入配置，例如input action那些。还有摄像机模式 camera mode

```cpp
UCLASS(MinimalAPI, BlueprintType, Const, Meta = (DisplayName = "Loch Pawn Data", ShortTooltip = "Data asset used to define a Pawn."))
class ULochPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UE_API ULochPawnData(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loch|Pawn")
	TSubclassOf<APawn> PawnClass;
};
```

到这里我们只配置了体验，还没有加载体验呢。但是首先要把game mode 几个主要类创建一下

先创建 game mode，然后按这个关系把空类创建出来。

```cpp
ALochGameMode::ALochGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
    GameStateClass = ALochGameState::StaticClass();
    GameSessionClass = ALochGameSession::StaticClass();
    PlayerControllerClass = ALochPlayerController::StaticClass();
	ReplaySpectatorPlayerControllerClass = ALochReplayPlayerController::StaticClass();
	PlayerStateClass = ALochPlayerState::StaticClass();
	DefaultPawnClass = ALochCharacter::StaticClass();
	HUDClass = ALochHUD::StaticClass();
}
```
搞一个 ALochGameMode -> B_LochGameMode(蓝图) 然后project settings 设置一下。把相关的类都设置一下，创建一个空类，但是什么都不写。

关联的 move ment component, ability system 也都是空类，先过编译。不指望任何流程生效。然后就不能动了，game mode 生效。

## 6. 体验管理器

ULochExperienceManager 只是一个引用计数记录

ULochExperienceManagerComponent 是一个 game state component。

```cpp
UCLASS(MinimalAPI, Config = Game)
class ALochGameState : public AModularGameStateBase, public IAbilitySystemInterface
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TObjectPtr<ULochExperienceManagerComponent> ExperienceManagerComponent;
};
```
并且在构造函数要 `ExperienceManagerComponent = CreateDefaultSubobject<ULochExperienceManagerComponent>(TEXT("..."));`初始化

他的加载位于game mode 的 init game 里面

```cpp
void ALochGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);
}
```

选择 experience 的逻辑位于 `HandleMatchAssignmentIfNotExpectingOne` 里面
```cpp
void ALochGameMode::HandleMatchAssignmentIfNotExpectingOne()
{
	FPrimaryAssetId ExperienceId;
	FString ExperienceIdSource;

	// 优先级顺序 (最高优先级胜出)
	//  - 匹配分配 (如果存在)
	//  - URL 选项覆盖 (URL Options override)
	//  - 开发者设置 (仅 PIE 模式)
	//  - 命令行覆盖 (Command Line override)
	//  - 世界设置 (World Settings)
	//  - 专用服务器 (Dedicated server)
	//  - 默认体验 (Default experience)

	UWorld* World = GetWorld();

	// 1. 检查 URL 参数中是否有 "Experience" 选项 (例如: ?Experience=B_MyExperience)
	// 2. 检查命令行参数中是否有 "Experience=" 设置 (例如: -Experience=B_MyExperience)
	// 3. 检查当前关卡的世界设置 (World Settings) 中是否配置了默认 Experience
	// 验证找到的 Experience 是否有效 (是否存在于 AssetManager 中)
	// 4. 最终回退方案：使用默认 Experience

	// 确定了 Experience 后，通知 GameMode 进行处理 (加载并应用)
	OnMatchAssignmentGiven(ExperienceId, ExperienceIdSource);
}
```

然后就是 
ExperienceComponent->SetCurrentExperience(ExperienceId) -> StartExperienceLoad
```cpp
void ULochExperienceManagerComponent::StartExperienceLoad()
{
    // 客户端 OnRep_CurrentExperience->StartExperienceLoad也会这么走到这里
    // 加载资产 ...
}
```
TODO 资产加载流程总结一下 ChangeBundleStateForPrimaryAssets 和 LoadAssetList
加载完成到了 `ULochExperienceManagerComponent::OnExperienceLoadComplete`

```cpp
void ULochExperienceManagerComponent::OnExperienceLoadComplete()
{
    // 验证plugin URL是否有效
    // 加载并激活 GameFeaturePlugins
}
```

最后是`ULochExperienceManagerComponent::OnExperienceFullLoadCompleted` 处理一些回调后，出发相关代理

```cpp
void ULochExperienceManagerComponent::OnExperienceFullLoadCompleted()
{
	// 可能的延迟逻辑 ...

	LoadState = ELochExperienceLoadState::ExecutingActions;

    // ...
    // 遍历 UGameFeatureAction 触发 OnGameFeatureRegistering, OnGameFeatureLoading, OnGameFeatureActivating

	LoadState = ELochExperienceLoadState::Loaded;

    // 按优先级广播加载完成事件
	OnExperienceLoaded_HighPriority.Broadcast(CurrentExperience);
	OnExperienceLoaded_HighPriority.Clear();

	OnExperienceLoaded.Broadcast(CurrentExperience);
	OnExperienceLoaded.Clear();

	OnExperienceLoaded_LowPriority.Broadcast(CurrentExperience);
	OnExperienceLoaded_LowPriority.Clear();
}
```

另外 `ULochExperienceManagerComponent::EndPlay` 里面会处理卸载逻辑

