// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FanMeetingDemoGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class FANMEETINGDEMO_API AFanMeetingDemoGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
	UFUNCTION(BlueprintCallable)
		void ForcedMuteAllOnOff();
	

	UPROPERTY(BlueprintReadWrite)
		bool ForcedMute = false;
};
