// Copyright Epic Games, Inc. All Rights Reserved.

//custom header
#include "FanMeetingPlayerState.h"

//unreal header
#include "Kismet/GameplayStatics.h"
#include "FanMeetingDemoGameModeBase.h"


//void AFanMeetingDemoGameModeBase::PostLogin(APlayerController* NewPlayer)
//{
//	Super::PostLogin(NewPlayer);
//	
//	PlayerName = UGameplayStatics::ParseOption(OptionsString, "PlayerName");
//	PlatformType = UGameplayStatics::ParseOption(OptionsString, "PlatformType");
//	AFanMeetingPlayerState* PlayerState = Cast<AFanMeetingPlayerState>(NewPlayer->PlayerState);
//	if (PlayerState != nullptr)
//	{
//		PlayerState->SetPlayerName(PlayerName);
//		PlayerState->SetPlatformType(PlatformType);
//	}
//}
