#pragma once

#include "CommonGameViewportClient.h"

#include "LochGameViewportClient.generated.h"

class UGameInstance;
class UObject;

UCLASS(BlueprintType)
class ULochGameViewportClient : public UCommonGameViewportClient
{
	GENERATED_BODY()

public:
	ULochGameViewportClient();

	virtual void Init(struct FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice = true) override;
};
