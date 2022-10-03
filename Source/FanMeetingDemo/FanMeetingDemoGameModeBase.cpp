// Copyright Epic Games, Inc. All Rights Reserved.

#include "FanMeetingDemoGameModeBase.h"
//custom header
#include "Character/ParentCharacter.h"
#include "FanMeetingPlayerState.h"
//plugin header
#include "UniversalVoiceChatPro/Public/PlayerVoiceChatActor.h"
//unreal header
#include "Kismet/GameplayStatics.h"


void AFanMeetingDemoGameModeBase::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = NewPlayer;

    // VoiceChatActor ½ºÆù
    APlayerVoiceChatActor* VoiceActor = GetWorld()->SpawnActor<APlayerVoiceChatActor>(FVector(0, 0, 0), FRotator(0, 0, 0), SpawnParams);
    VoiceActor->ServerSetAttenuation(true, FString("/Game/NewSoundAttenuation.NewSoundAttenuation"));
}

void AFanMeetingDemoGameModeBase::ForcedMuteAllOnOff()
{
    TArray<AActor*> FindActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AParentCharacter::StaticClass(), FindActors);

    for (auto it : FindActors)
    {
        AParentCharacter* NowCharacter = Cast<AParentCharacter>(it);
        if (NowCharacter == nullptr) continue;

        AFanMeetingPlayerState* NowPlayerState = Cast<AFanMeetingPlayerState>(NowCharacter->GetPlayerState());
        if (NowPlayerState == nullptr) continue;

        if (NowPlayerState->GetJoinType().Compare("FAN") == 0)
        {
            if(ForcedMute) NowCharacter->ForcedMute = true;
            else NowCharacter->ForcedMute = false;
        }
    }
}
