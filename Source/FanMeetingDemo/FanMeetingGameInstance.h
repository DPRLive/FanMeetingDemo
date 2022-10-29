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

protected:
	UPROPERTY(BlueprintReadWrite)
		FString PlayerName ="UNKONWN_NAME";
	UPROPERTY(BlueprintReadWrite)
		uint8 PlatformType = -1; // 0:PC, 1:VR
	UPROPERTY(BlueprintReadWrite)
		FString JoinType = "NONE"; // FAN, MANAGER, VTUBER

public:
	void SetPlayerName(const FString& S) { PlayerName = S; }
	void SetPlatformType(const uint8& I) { PlatformType = I; }
	void SetJoinType(const FString& S) { JoinType = S; }

	FString GetPlayerName() { return PlayerName; }
	uint8 GetPlatformType() { return PlatformType; }
	FString GetJoinType() { return JoinType; }
};
