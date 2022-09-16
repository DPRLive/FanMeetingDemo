// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUI.h"
//plug in
#include "UniversalVoiceChatPro/Public/PlayerVoiceChatActor.h"
//custom
#include "../Character/PCCharacter.h"
#include "../Character/VRCharacter.h"
//ue header
#include "Kismet/GameplayStatics.h"

bool UInGameUI::Initialize()
{
	if (!Super::Initialize()) return false;
	return true;
}

void UInGameUI::GoMainMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Maps/MainMenu"));
	if (IsSetup()) Teardown();
}

void UInGameUI::Close()
{
	if (IsSetup()) Teardown();
}

void UInGameUI::MicOnOff()
{
	if (PlatformType == 0)
	{
		Cast<APCCharacter>(Owner)->VoiceChatOnOff();
	}
	else
		Cast<AVRCharacter>(Owner)->VoiceChatOnOff();
}

void UInGameUI::SetMicVolume(float Value)
{
	UUniversalVoiceChat::VoiceChatSetMicrophoneVolume(Value);
}
