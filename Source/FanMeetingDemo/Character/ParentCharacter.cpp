// Fill out your copyright notice in the Description page of Project Settings.

#include "../UI/NamePlate.h"

#include "UniversalVoiceChatPro/Public/PlayerVoiceChatActor.h"

#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "ParentCharacter.h"

AParentCharacter::AParentCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AParentCharacter::BeginPlay()
{
	Super::BeginPlay();
	UUniversalVoiceChat::VoiceChatSetMicrophoneVolume(5);

	FTimerHandle WaitHandle;
	// player state가 beginplay 하는 시점에 바로 생성이 안되는거 같음. 그래서 0.1초 기다리고 접근
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			NamePlateUpdate();
		}), 0.1, false);
}

void AParentCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AParentCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AParentCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AParentCharacter, PlayerName);
	DOREPLIFETIME(AParentCharacter, PlayerNameRef);
}

void AParentCharacter::NamePlateUpdate()
{
	if (IsLocallyControlled()) NamePlate->SetVisibility(false);
	else if (GetLocalRole() != ROLE_Authority) NamePlate->SetVisibility(true);
	if (HasAuthority())
	{
		PlayerNameRef = this->GetPlayerState()->GetPlayerName();
		OnRep_PlayerNameRef();
	}
	else
	{
		Cast<UNamePlate>(NamePlate->GetWidget())->SetCharacterRef(this);
	}
}

void AParentCharacter::OnRep_PlayerNameRef()
{
	PlayerName = PlayerNameRef;
}

void AParentCharacter::VoiceChatOnOff()
{
	if (IsVoiceChatOn)
	{
		UUniversalVoiceChat::VoiceChatStopSpeak();
		IsVoiceChatOn = false;
	}
	else
	{
		UUniversalVoiceChat::VoiceChatStartSpeak(true, true, 0, true, 1000);
		IsVoiceChatOn = true;
	}
}