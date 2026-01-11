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

GameFeatureAction_AddWidget
    参考 /ShooterCore/Experiences/LAS_ShooterGame_StandardHUD.LAS_ShooterGame_StandardHUD
    有layer和widget
    把这个推送到