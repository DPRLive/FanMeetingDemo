// Copyright Epic Games, Inc. All Rights Reserved.

#include "FanMeetingDemoGameModeBase.h"
//custom header

//plugin header
#include "UniversalVoiceChatPro/Public/PlayerVoiceChatActor.h"
//unreal header

void AFanMeetingDemoGameModeBase::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = NewPlayer;

    // VoiceChatActor ½ºÆù
    APlayerVoiceChatActor* VoiceActor = GetWorld()->SpawnActor<APlayerVoiceChatActor>(FVector(0, 0, 0), FRotator(0, 0, 0), SpawnParams);
    VoiceActor->ServerSetAttenuation(true, FString("/Game/NewSoundAttenuation.NewSoundAttenuation"));
}
