#include "LochGameData.h"
#include "LochAssetManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LochGameData)

ULochGameData::ULochGameData()
{
}

const ULochGameData& ULochGameData::ULochGameData::Get()
{
	return ULochAssetManager::Get().GetGameData();
}
