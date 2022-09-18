// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUI.h"
//plug in
#include "UniversalVoiceChatPro/Public/PlayerVoiceChatActor.h"
//custom
#include "../Character/ParentCharacter.h"
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
	Owner->VoiceChatOnOff();
}

void UInGameUI::SetMicVolume(float Value)
{
	UUniversalVoiceChat::VoiceChatSetMicrophoneVolume(Value);
}

void UInGameUI::BlinkerOnOff()
{
	if (PlatformType == 0) return;
	Cast<AVRCharacter>(Owner)->SetBlink();
}

void UInGameUI::ResetVR()
{
	if (PlatformType == 0) return;
	Cast<AVRCharacter>(Owner)->OnResetVR();
}

void UInGameUI::TogglePlayerName()
{
	if (CharacterClass == nullptr) return;

	TArray<AActor*> Array;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), CharacterClass, Array);

	if (ShowPlayerName)
	{
		for (int32 i = 0; i < Array.Num(); i++)
		{
			Cast<AParentCharacter>(Array[i])->GetNamePlate()->SetVisibility(false);
		}
		ShowPlayerName = false;
	}
	else
	{
		for (int32 i = 0; i < Array.Num(); i++)
		{
			Cast<AParentCharacter>(Array[i])->GetNamePlate()->SetVisibility(true);
		}
		ShowPlayerName = true;
	}
}
