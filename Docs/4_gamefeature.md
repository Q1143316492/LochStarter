# GameFeatures

配置这里有个tag
Plugins\GameFeatures\ShooterCore\Config\Tags

Engine\Plugins\Runtime\GameFeatures\Source\GameFeatures\Private\GameFeaturePluginStateMachine.h
这里有个加载状态图

game feature action 与 自定义action
[
    game feature add comp
    game feature add cheats

]

## chunk ID 覆写

asset mgr Primaryasstlabel 那个配置

对应目录配置是这个data asset
Content/DefaultGame_Label
game feature目录的content, maps, topdown 也有一个

chunkID 代表资产打到哪个包

## 一些Action TODO

GameFeatureAction_WorldActionBase

GameFeatureAction_AddAbility
    写到这里 ULyraAbilitySet 要加上了
    LyraAbility

为了搞ability加出了一堆东西梳理一下

### PawnData

玩家体验配置pawn data的DA，里面是 Input Config，ability set, tag relation ship mapping ,camera mode.

- InputConfig

LochInputConfig.h 包括增强输入的input action的维护DA。action还分了native输入和技能输入
LochInputComponent.cpp UEnhancedInputComponent子类，包装了一些方法来适配项目LochInputConfig的输入结构到增强输入系统，实际上没什么东西。需要去项目设置设置一下啊

- GameFeatureAction_AddAbilities

可配置 ActorClass 添加 FLochAbilityGrant(UGameplayAbility类型的包装) 和 FLochAttributeSetGrant(UAttributeSet类型的包装) 结构
或者是 ULochAbilitySet(DA) 包括 GA, GE, attr set 的组合，相当于一个职业的技能包

这里的 ULochGameplayAbility 引出了很多东西

技能类:

Cost相关类  都在 Ability目录下

ULochAbilitySet GA, GE, attr set 的组合，相当于一个职业的技能包

ULochAbilitySourceInterface 根据物理材质计算技能衰减的ability类，先不管

ULochAbilityTagRelationshipMapping 技能中有技能A标签，就不能是否技能B标签，要取消技能C标签，做这样的逻辑的一个描述关系的DA

FLochGameplayEffectContext 看起来是几种不同物理效果功能的对于 ULochAbilitySourceInterface 对象的一个逻辑，暂时还不清楚什么用

ULochGameplayAbility 好大以类
    技能的摄像机模式 ULochCameraMode 里面是一些描述摄像机的属性
    ELochAbilityActivationPolicy 技能激活策略(瞬发，持续，生成式？目前是猜的)
    ELochAbilityActivationGroup 技能组，独立，能被替换，能阻止其他技能
    AdditionalCosts 技能的花费
    技能失败播放的蒙太奇map

ULochAbilitySystemComponent
    TagRelationshipMapping 描述技能的关系的映射，谁能取消谁，谁能阻止谁

摄像机相关的
    CameraMode 摄像机的数据结构
    ALochPlayerCameraManager 主要目的好像是中转 UI debug内容的
    ULochUICameraManagerComponent 画面UI摄像机的组件，是个
    CameraComponent 继承 UCameraComponent

物品定义
    LochInventoryItemDefinition 物品名字和 片段？？？目前看不出啥

输入
    ULochGameFeatureAction_AddInputContextMapping

角色组件（看起来是拿来分化character逻辑的）
    ULochPawnExtensionComponent 缓存了技能组件，和pawn data。是一个 PawnComponent，和提供IGameFrameworkInitStateInterface接口
    ULochHeroComponent 输入逻辑，初始化状态


GameFeatureAction_AddInputBinding
    这里还设计补全hero component的部分内容

UGameFeatureAction_AddInputContextMapping

OK 提交一次

## GameFeatureAction_AddWidget

    参考 /ShooterCore/Experiences/LAS_ShooterGame_StandardHUD.LAS_ShooterGame_StandardHUD
    有layer或者widget 加一个tag的配置组成，应该是处于配置分离的目录拆分的，
    核心推送逻辑 UGameFeatureAction_AddWidgets::AddWidgets

核心的触发逻辑
```cpp
void UGameFeatureAction_AddWidgets::AddWidgets(AActor* Actor, FPerContextData& ActiveData)
{
	ALyraHUD* HUD = CastChecked<ALyraHUD>(Actor);
    // ...

	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(HUD->GetOwningPlayerController()->Player))
	{
		FPerActorData& ActorData = ActiveData.ActorData.FindOrAdd(HUD);

		for (const FLyraHUDLayoutRequest& Entry : Layout)
		{
			if (TSubclassOf<UCommonActivatableWidget> ConcreteWidgetClass = Entry.LayoutClass.Get())
			{
				ActorData.LayoutsAdded.Add(UCommonUIExtensions::PushContentToLayer_ForPlayer(LocalPlayer, Entry.LayerID, ConcreteWidgetClass));
			}
		}

		UUIExtensionSubsystem* ExtensionSubsystem = HUD->GetWorld()->GetSubsystem<UUIExtensionSubsystem>();
		for (const FLyraHUDElementEntry& Entry : Widgets)
		{
			ActorData.ExtensionHandles.Add(ExtensionSubsystem->RegisterExtensionAsWidgetForContext(Entry.SlotID, LocalPlayer, Entry.WidgetClass.Get(), -1));
		}
	}
}
```

推送布局
PushContentToLayer_ForPlayer 
UI subsystem 有一个策略，然后创建根布局。

看到拿到 UGameUIManagerSubsystem 的 policy。用policy的 root layout 来推送widget
```cpp
UCommonActivatableWidget* UCommonUIExtensions::PushContentToLayer_ForPlayer(const ULocalPlayer* LocalPlayer, FGameplayTag LayerName, TSubclassOf<UCommonActivatableWidget> WidgetClass)
{
	if (!ensure(LocalPlayer) || !ensure(WidgetClass != nullptr))
	{
		return nullptr;
	}

	if (UGameUIManagerSubsystem* UIManager = LocalPlayer->GetGameInstance()->GetSubsystem<UGameUIManagerSubsystem>())
	{
		if (UGameUIPolicy* Policy = UIManager->GetCurrentUIPolicy())
		{
			if (UPrimaryGameLayout* RootLayout = Policy->GetRootLayout(CastChecked<UCommonLocalPlayer>(LocalPlayer)))
			{
				return RootLayout->PushWidgetToLayerStack(LayerName, WidgetClass);
			}
		}
	}

	return nullptr;
}
```

这个policy是什么，是ui susbsystem 里配置的
UGameUIManagerSubsystem::NotifyPlayerAdded 在local player 加入的时候会调用policy
UGameUIPolicy::NotifyPlayerAdded
其中
`TSoftClassPtr<UPrimaryGameLayout> LayoutClass;`
是有那个多个layout层级的界面，这个根布局的创建最后回到
```cpp 
void UGameUIPolicy::AddLayoutToViewport(UCommonLocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout)
{
	UE_LOG(LogCommonGame, Log, TEXT("[%s] is adding player [%s]'s root layout [%s] to the viewport"), *GetName(), *GetNameSafe(LocalPlayer), *GetNameSafe(Layout));

	Layout->SetPlayerContext(FLocalPlayerContext(LocalPlayer));
	Layout->AddToPlayerScreen(1000);

	OnRootLayoutAddedToViewport(LocalPlayer, Layout);
}
```

UUserWidget -> UCommonUserWidget -> UCommonActivatableWidget -> ULyraActivatableWidget
UUserWidget -> UCommonUserWidget ->  UPrimaryGameLayout

UCommonUserWidget:
来源: CommonUI 插件 (CommonUserWidget.h)
定位: UUserWidget 的增强版，修复/增强了基础输入功能。
它做了什么:
- 输入路由地基: 它是 CommonUI 输入系统的基本单元。
- 动作绑定 (Action Binding): 提供了 RegisterUIActionBinding，允许你用代码更方便地绑定手柄/键盘按键（比如“按下 A 键确认”）。
- 指针输入控制: 提供了 SetConsumePointerInput 来更好地控制鼠标/触摸点击是否穿透。
- 滚动管理: 管理 Gamepad 模拟摇杆滚动列表的接收者。

UCommonActivatableWidget:
来源: CommonUI 插件 (CommonActivatableWidget.h)
定位: 核心概念类。代表一个“屏幕”或“菜单页面”及“生命周期”。
它做了什么:
- 激活/未激活状态 (Activation): 引入了 Activated 和 Deactivated 概念。
- 当你 Push 一个界面入栈，它变为 Active（获取焦点，接收输入）。
- 当你 Push 另一个界面盖在它上面，它变为 Inactive（失去焦点，暂停输入响应，但可视）。
- 当你 Pop 顶层界面，下面的界面重新变为 Active。
- 返回键处理: 自动集成了“Back”逻辑（ESC 或 手柄B键），可以配置为自动关闭自己。
- 焦点恢复: 自动记录上一次选中的按钮，当界面重新激活时，自动把焦点还给那个按钮（非常适合手柄）。

来源: LyraActivatableWidget.h (或 LochStarter 对应版本)
定位: 项目特定的基类，连接 UI 系统和 Gameplay 系统。
它做了什么:
- 输入模式配置 (GetDesiredInputConfig):
- 這是最关键的一点。它覆写了这个函数来告诉 PlayerController：“当我在显示时，输入模式应该是怎样的？”
- 它定义了 ELyraWidgetInputMode (例如: Game, Menu, GameAndMenu)。
- 例子: 当你打开“设置菜单”时，这个类会自动告诉游戏层：“现在显示鼠标光标，并且忽略角色的移动输入”。当你关闭它时，自动恢复回去。
- 鼠标捕获模式: 定义了当此界面激活时，鼠标是否应该锁定在视口内 (CapturePermanently, CaptureDuringMouseDown, etc.)。

UI 设计师 只关心 UUserWidget (在编辑器里摆按键)。
UI 程序员 关心 UCommonActivatableWidget (管理页面的进出栈逻辑)。
Gameplay 程序员 关心 ULyraActivatableWidget (确保打开背包时，角色不会因为乱按键盘而开枪)。

UCommonUserWidget -> UPrimaryGameLayout 
UPrimaryGameLayout:
职责:
它是**“UI 的根节点”**。每个玩家可以有一个（LocalPlayer 级别）。
它不是一个具体的菜单，而是一个容器。
它管理着 Layers (层级)。通常这里面会放几个 UCommonActivatableWidgetContainer（栈）。
例如：Lyra默认配置了 Game, GameMenu, Menu, Modal 四层。当你 Push 一个菜单时，实际上是在调用 PrimaryGameLayout 把它推到某一层的栈顶


ULyraActivatableWidget -> ULyraHUDLayout
ULyraHUDLayout:
职责:
它是 Game 层 的主要内容。
当游戏开始时，GameplayAbility 或 Experience 会请求 "Push HUD"。
这个类通常是空的或者只是个容器，里面通过 UCommonActivatableWidgetContainer 再去加载具体的血条、弹药栏等子控件。

UCommonButtonBase 解决了 UMG 默认按钮无法很好处理手柄导航、选中状态保持、悬停动画统一样式等问题

在这个体系下，它们是这样协作的：
UPrimaryGameLayout (作为根) 被添加到视口。
它内部有几个 Stack (UI栈)。
你把 ULyraActivatableWidget (比如设置菜单) Push 进 Menu Stack。
你的设置菜单里有一些 UCommonButtonBase (比如“保存”按钮)。
所以，UPrimaryGameLayout 是“容器”，ULyraActivatableWidget 是“内容”，UCommonButtonBase 是“零件”。

==
“异步 UI 框架”在 Lyra/LochStarter (基于 CommonUI) 体系下，主要指的是 为了解决“打开大菜单卡顿”和“内存占用过高” 而设计的一整套 资源懒加载 (Lazy Loading) 机制。

它的核心理念是：不要在游戏一开始就加载所有 UI 这种“硬引用” (Hard Reference)。而是只存一个“软引用” (Soft Reference/Path)，等到玩家真正按键要打开那个菜单时，才去异步加载它，加载完再显示。

这套框架由以下几个关键部分组成：

1. 核心载体：TSoftClassPtr (软类引用)
在传统开发中，你可能在 HUD 里直接放一个 UPROPERTY() TSubclassOf<UUserWidget> PauseMenuClass;。
这会导致：游戏启动加载 HUD 时，会顺藤摸瓜把 PauseMenuClass 也加载进内存。如果 PauseMenu 引用了商城、装备、贴图，那内存就爆了，启动也慢。

在异步 UI 框架中，所有 UI 引用都变成了：
UPROPERTY() TSoftClassPtr<UCommonActivatableWidget> PauseMenuClass;
这意味着：我知道那个菜单文件在哪，但我还没加载它。

2. 执行者：UAsyncAction_PushContentToLayerForPlayer (Blueprint 节点)
这是你在蓝图中见到的那个带时钟图标的节点。

输入: 它接收一个 SoftClassPtr（还没加载的软引用）。
过程:
它会告诉 StreamableManager：“去后台加载这个 Widget 类”。
（可选）此时会挂起输入 (bSuspendInputUntilComplete = true)，防止玩家在没加载出来前乱按。
加载完成后，触发委托。
调用 PrimaryGameLayout 把它 Push 到屏幕上。
代码位置: AsyncAction_PushContentToLayerForPlayer.h
3. 管理者：UPrimaryGameLayout 的 PushWidgetToLayerStackAsync
虽然你看之前的代码片段（UGameFeatureAction_AddWidgets）里用的是同步的 PushContentToLayer_ForPlayer，但那是因为 GameFeatures 开启时，Unreal 的 Bundle 机制通常已经把这个 Feature 里的资产（在 Loading 阶段）预加载好了。

但在运行时（比如按 ESC 弹菜单），则是走 PrimaryGameLayout 的异步通道：

它内部维护了一个 Loading State，确保在 UI 加载出来之前，输入不会“穿透”到游戏世界里，也不会让游戏逻辑因为等待 IO 而卡死（Hitches）。

总结
这套“异步 UI 框架”实际上是 CommonUI + CommonGame 提供的设计模式：

传统 UI 写法	异步 UI 框架写法
引用	TSubclassOf<UWidget> (硬引用)
加载时机	随父级（如 HUD 或 PlayerController）一起加载
打开方式	CreateWidget -> AddToViewport
结果	启动慢，内存大，但打开瞬间快
所以，当你看到文档或代码里提到“Async UI”时，通常就是在说：用 Async Action 节点去把软引用的 UI 加载并 Push 到 PrimaryGameLayout 的某个 Layer 这种工作流
==

`class UCommonUIExtensions : public UBlueprintFunctionLibrary`

UCommonActivatableWidgetContainerBase
用于管理 N 个可激活小部件的小部件基础，一次显示一个。
有意设计为黑盒，不像普通面板小部件那样暴露子部件 / 插槽修改功能。

## GameFeatureAction_AddWidget

## GameFeatureAction_SplitScreenConfig

```cpp
/**
 * A game viewport (FViewport) is a high-level abstract interface for the
 * platform specific rendering, audio, and input subsystems.
 * GameViewportClient is the engine's interface to a game viewport.
 * Exactly one GameViewportClient is created for each instance of the game.  The
 * only case (so far) where you might have a single instance of Engine, but
 * multiple instances of the game (and thus multiple GameViewportClients) is when
 * you have more than one PIE window running.
 *
 * Responsibilities:
 * propagating input events to the global interactions list
 *
 * @see UGameViewportClient
 */
UCLASS(Within=Engine, transient, config=Engine, MinimalAPI)
class UGameViewportClient : public UScriptViewportClient, public FExec
{
	GENERATED_UCLASS_BODY()

public:
	/** Allows game code to disable splitscreen (useful when in menus) */
	ENGINE_API void SetForceDisableSplitscreen(const bool bDisabled);
}
```
SetForceDisableSplitscreen 基于这个做的action

## GameFeatureAction_AddGameplayCuePath & LochGameFeaturePolicy

从这里开始调用 ULochAssetManager::InitializeGameplayCueManager
补全 ULochGameplayCueManager 类，当然这是一个 DA
然后是 LochGameFeaturePolicy 文件

引用类 hotfix/hotfix mgr 先搞出来

注意这里有一个配置

```
[/Script/GameplayAbilities.AbilitySystemGlobals]
GlobalGameplayCueManagerClass=/Script/LochStarterGame.LochGameplayCueManager
GlobalGameplayCueManagerName=None
```

没有这个 cue manager 是空，当然目前我们ability也没搞

LochGameFeaturePolicy 这个主要是提供了一些mgr的回调，为game feature 时机做一些定制化处理

## ULyraRuntimeOptions

## ULyraTextHotfixConfig


