# Asset Manager

## 1. 基础属性和初始化

```cpp
UCLASS(MinimalAPI, Config = Game)
class ULochAssetManager : public UAssetManager
{
	GENERATED_BODY()
protected:
	UPROPERTY(Config)
	TSoftObjectPtr<ULochGameData> LochGameDataPath;

	UPROPERTY(Transient)
	TMap<TObjectPtr<UClass>, TObjectPtr<UPrimaryDataAsset>> GameDataMap;

	UPROPERTY(Config)
	TSoftObjectPtr<ULochPawnData> DefaultPawnData;
};
```
最关键的其实是这三，
game data 里面是一些 gas 的 gameplay effect
pawn data 前面提到过，其他的不好归类不常用丢到一个map里面

这个也是要在设置里面配一下，然后就写进ini里面了

嵌入引擎的流程
```cpp
	UE_API virtual void StartInitialLoading() override;
#if WITH_EDITOR
	UE_API virtual void PreBeginPIE(bool bStartSimulate) override;
#endif
```

```cpp
void ULochAssetManager::StartInitialLoading()
{
	SCOPED_BOOT_TIMING("ULochAssetManager::StartInitialLoading"); // 分析用的

	// This does all of the scanning, need to do this now even if loads are deferred
    // 完成扫描，即使加载被推迟也需要完成扫描
	Super::StartInitialLoading();

	STARTUP_JOB(InitializeGameplayCueManager());

	{
		// Load base game data asset
		STARTUP_JOB_WEIGHTED(GetGameData(), 25.f);
	}

	// Run all the queued up startup jobs
	DoAllStartupJobs();
}
```
STARTUP_JOB, STARTUP_JOB_WEIGHTED 定义了一个 job。把函数放在一个lambda传进去一个结构。并加到一个array里面。
在 DoAllStartupJobs 里面执行，逻辑比较简单，如果是DS只需要do job。否则会有进度显示回调。但是实际上是留空的，没有用上。

人话就是，把回调存起来一起执行。包装了一下，能够显示进度。

## 2. 加载 GameData 和 PawnData

### 2.1 资源加载

FSoftObjectPath 是软引用路径. 如 `/Game/Path/To/YourAsset.YourAsset`

```cpp
FSoftObjectPath Path(TEXT("/Game/Path/To/YourAsset.YourAsset"));
UObject* LoadedObject = Path.TryLoad(); // 同步加载
```

TSoftObjectPtr<T> 是一个模板类，封装了 FSoftObjectPath，并提供了一些便捷的方法。

```cpp
TSoftObjectPtr<ULochGameData> GameDataPtr(TEXT("/Game/Path/To/YourGameData.YourGameData"));
ULochGameData* GameData = GameDataPtr.LoadSynchronous(); // 同步加载，本质也是 TryLoad
``` 

模板的内部是 FSoftObjectPtr
```cpp
template<class T=UObject>
struct TSoftObjectPtr
{
private:
	FSoftObjectPtr SoftObjectPtr;
}
```

如何异步加载？
FStreamableManager, LoadAsync 等。

### 2.2 具体实现

```cpp
const ULochPawnData* ULochAssetManager::GetDefaultPawnData() const
{
	return GetAsset(DefaultPawnData);
}
```

```cpp
template<typename AssetType>
AssetType* ULochAssetManager::GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	AssetType* LoadedAsset = nullptr;
	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

    // 拿到路径
	if (AssetPath.IsValid())
	{
		LoadedAsset = AssetPointer.Get();
		if (!LoadedAsset)
		{
            // 加载
			LoadedAsset = Cast<AssetType>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedAsset, TEXT("Failed to load asset [%s]"), *AssetPointer.ToString());
		}

		if (LoadedAsset && bKeepInMemory)
		{
			// Added to loaded asset list.
			Get().AddLoadedAsset(Cast<UObject>(LoadedAsset));
		}
	}
	return LoadedAsset;
}
```

另一个

```cpp
const ULochGameData& ULochAssetManager::GetGameData()
{
	return GetOrLoadTypedGameData<ULochGameData>(LochGameDataPath);
}
```

```cpp
template <typename GameDataClass>
const GameDataClass& GetOrLoadTypedGameData(const TSoftObjectPtr<GameDataClass>& DataPath)
{
    if (TObjectPtr<UPrimaryDataAsset> const * pResult = GameDataMap.Find(GameDataClass::StaticClass()))
    {
        return *CastChecked<GameDataClass>(*pResult);
    }

    // Does a blocking load if needed
    return *CastChecked<const GameDataClass>(LoadGameDataOfClass(GameDataClass::StaticClass(), DataPath, GameDataClass::StaticClass()->GetFName()));
}
```

```cpp
UPrimaryDataAsset* ULochAssetManager::LoadGameDataOfClass(TSubclassOf<UPrimaryDataAsset> DataClass, const TSoftObjectPtr<UPrimaryDataAsset>& DataClassPath, FPrimaryAssetType PrimaryAssetType)
{
	UPrimaryDataAsset* Asset = nullptr;
	if (!DataClassPath.IsNull())
	{
        // ...

        if (GIsEditor)
		{
			Asset = DataClassPath.LoadSynchronous();
			LoadPrimaryAssetsWithType(PrimaryAssetType);
		}
		else
		{
			TSharedPtr<FStreamableHandle> Handle = LoadPrimaryAssetsWithType(PrimaryAssetType);
			if (Handle.IsValid())
			{
				Handle->WaitUntilComplete(0.0f, false);
				Asset = Cast<UPrimaryDataAsset>(Handle->GetLoadedAsset());
			}
		}
	}

	if (Asset) { GameDataMap.Add(DataClass, Asset); } else { /* LOG */ }
	return Asset;
}
```

其他的业务用到了在配置吧

# GameInstance

主要是这里有个加密的流程，去断点看看

- HandlerUserInitialized 点击搜索玩家的时候会到这里

UCommonUserSubsystem::HandleUserInitializeSucceeded
ULyraGameInstance::HandlerUserInitialized

- 第二个玩家加入房间的时候

UCommonSessionSubsystem::ConnectToHostReservationBeacon
UCommonSessionSubsystem::InternalTravelToSession
ULyraGameInstance::OnPreClientTravelToSession
这里的URL是 IP:7777

其他的比较简单，直接能跑。这个加密没跑进来。

# local player & game viewport client

内容先空着，别忘了去project settings 里面把 local player class 和 game viewport client class 设置一下

# 开发者设置

Editor Preferences -> Lyra Develop Settings那个面板

Development/LochDeveloperSettings.cpp 这个相当于只是一个配置，具体用到的地方在具体写读值怎么用。

DefaultEditorPerProjectUserSettings 这个扣过来

和类名有关
```cpp
UCLASS(config=EditorPerProjectUserSettings, MinimalAPI)
class ULochDeveloperSettings : public UDeveloperSettingsBackedByCVars {}
```

启动会报错 LochPC.ShouldAlwaysPlayForceFeedback 未找到

```cpp
namespace Loch
{
	namespace Input
	{
		static int32 ShouldAlwaysPlayForceFeedback = 0;
		static FAutoConsoleVariableRef CVarShouldAlwaysPlayForceFeedback(TEXT("LochPC.ShouldAlwaysPlayForceFeedback"),
			ShouldAlwaysPlayForceFeedback,
			TEXT("Should force feedback effects be played, even if the last input device was not a gamepad?"));
	}
}
```
他需要你去用到的地方把命令加上，例如这里是player controller cpp的头上

# 平台设置

PlatformEmulationSettings

# LochPlayerStarter

ULochPlayerSpawningManagerComponent 会有这个流程加载玩家，是game feature动态加上去的。

# UGameUIManagerSubsystem

```cpp
int32 UCommonGameInstance::AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId)
{
	int32 ReturnVal = Super::AddLocalPlayer(NewPlayer, UserId);
	if (ReturnVal != INDEX_NONE)
	{
		if (!PrimaryPlayer.IsValid())
		{
			UE_LOG(LogCommonGame, Log, TEXT("AddLocalPlayer: Set %s to Primary Player"), *NewPlayer->GetName());
			PrimaryPlayer = NewPlayer;
		}
		
		GetSubsystem<UGameUIManagerSubsystem>()->NotifyPlayerAdded(Cast<UCommonLocalPlayer>(NewPlayer));
	}
	
	return ReturnVal;
}
```
没有这个东西，就炸了，先补一个

Game/UI
W_OverallUILayout 是一个层级的文件

B_LyraUIPolicy 是一个 policy文件。由于基类是插件的，所以蓝图是好的。

但是我们简单改个名，避免某些东西莫名其妙是正常的。然后 DefaultGame.ini 就这么配吧

```ini
[/Script/LochStarterGame.LochUIManagerSubsystem]
DefaultUIPolicyClass=/Game/UI/B_LochUIPolicy.B_LochUIPolicy_C
```

然后又找不到 experience definition 预计是 asset manager 里面是扫描路径没配对，检查一下。




