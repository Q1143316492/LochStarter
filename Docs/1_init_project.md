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
    - AsyncMixin 优化我需要加载A和B才能开始工作

-  UI 扩展与通用功能

    - UIExtension
    - CommonGame
    - CommonUser
    - CommonLoadingScreen
    - GameSettings

- 模块化游戏功能

    - GameFeatures 不用管，后面创建的时候会有的
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

## 5. Gameplay 类

Lyra的新概念，应该是world settings配置体验，体验里面配置pawn data。

pawn data 里面是技能，输入，摄像机。先顺着这个把game play 的几个核心类搞起来。

- 自定义 Engine 类 LochStarterEditorEngine，ULochStarterGameEngine 

    - 目前没实际作用。可以默认打开Plugins目录，如果是PIE且world settings设置了单机模式，就强制PIE单机模式。 应该是给那个炸弹人用的。但是现在对我们没用。简单扣过来。

    - DefaultEngine.ini [/Script/Engine.Engine] 里面配置，断点一下没问题提交一次


