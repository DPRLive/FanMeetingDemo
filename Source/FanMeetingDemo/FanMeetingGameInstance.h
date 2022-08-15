// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FanMeetingGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class FANMEETINGDEMO_API UFanMeetingGameInstance : public UGameInstance
{
	GENERATED_BODY()
private:
	FString PlayerName;
	uint8 PlatformType; // 0:PC, 1:VR

public:
	void SetPlayerName(const FString& S) { PlayerName = S; }
	void SetPlatformType(const uint8& I) { PlatformType = I; }

	FString GetPlayerName() { return PlayerName; }
	uint8 GetPlatformType() { return PlatformType; }
};
