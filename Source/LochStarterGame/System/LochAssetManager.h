#pragma once

#include "Engine/AssetManager.h"
#include "LochAssetManagerStartupJob.h"
#include "Templates/SubclassOf.h"
#include "LochAssetManager.generated.h"

#define UE_API LOCHSTARTERGAME_API

class UPrimaryDataAsset;

class ULochGameData;
class ULochPawnData;

struct FLochBundles
{
	static const FName Equipped;
};


UCLASS(MinimalAPI, Config = Game)
class ULochAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	UE_API ULochAssetManager();
    static UE_API ULochAssetManager& Get();

	template<typename AssetType>
	static AssetType* GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	template<typename AssetType>
	static TSubclassOf<AssetType> GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

    // Logs所有通过资产管理器加载和跟踪的所有资产。
    static UE_API void DumpLoadedAssets();

	UE_API const ULochGameData& GetGameData();
	UE_API const ULochPawnData* GetDefaultPawnData() const;

protected:
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

    static UE_API UObject* SynchronousLoadAsset(const FSoftObjectPath& AssetPath);
    static UE_API bool ShouldLogAssetLoads();

    // 线程安全的加入内存
    UE_API void AddLoadedAsset(const UObject* Asset);

	//~UAssetManager interface
	UE_API virtual void StartInitialLoading() override;
#if WITH_EDITOR
	UE_API virtual void PreBeginPIE(bool bStartSimulate) override;
#endif
	//~End of UAssetManager interface

    UE_API UPrimaryDataAsset* LoadGameDataOfClass(TSubclassOf<UPrimaryDataAsset> DataClass, const TSoftObjectPtr<UPrimaryDataAsset>& DataClassPath, FPrimaryAssetType PrimaryAssetType);


private:
	UE_API void DoAllStartupJobs();
    UE_API void InitializeGameplayCueManager();
    UE_API void UpdateInitialGameContentLoadPercent(float GameContentPercent);

protected:
	UPROPERTY(Config)
	TSoftObjectPtr<ULochGameData> LochGameDataPath;

	UPROPERTY(Transient)
	TMap<TObjectPtr<UClass>, TObjectPtr<UPrimaryDataAsset>> GameDataMap;

	UPROPERTY(Config)
	TSoftObjectPtr<ULochPawnData> DefaultPawnData;

private:
	// Assets loaded and tracked by the asset manager.
	UPROPERTY()
	TSet<TObjectPtr<const UObject>> LoadedAssets;

	// Used for a scope lock when modifying the list of loaded assets.
	FCriticalSection LoadedAssetsCritical;

private:
    TArray<FLochAssetManagerStartupJob> StartupJobs;
};

template<typename AssetType>
AssetType* ULochAssetManager::GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	AssetType* LoadedAsset = nullptr;
	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		LoadedAsset = AssetPointer.Get();
		if (!LoadedAsset)
		{
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

template<typename AssetType>
TSubclassOf<AssetType> ULochAssetManager::GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	TSubclassOf<AssetType> LoadedSubclass;
	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		LoadedSubclass = AssetPointer.Get();
		if (!LoadedSubclass)
		{
			LoadedSubclass = Cast<UClass>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedSubclass, TEXT("Failed to load asset class [%s]"), *AssetPointer.ToString());
		}

		if (LoadedSubclass && bKeepInMemory)
		{
			// Added to loaded asset list.
			Get().AddLoadedAsset(Cast<UObject>(LoadedSubclass));
		}
	}

	return LoadedSubclass;
}

#undef UE_API