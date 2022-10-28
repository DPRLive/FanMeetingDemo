// Fill out your copyright notice in the Description page of Project Settings.

#include "ParentCharacter.h"
#include "../UI/NamePlate.h"

#include "UniversalVoiceChatPro/Public/PlayerVoiceChatActor.h"

#include "Net/UnrealNetwork.h"

AParentCharacter::AParentCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AParentCharacter::BeginPlay()
{
	Super::BeginPlay();
	UUniversalVoiceChat::VoiceChatSetMicrophoneVolume(5);

	FTimerHandle WaitHandle;
	// player state�� beginplay �ϴ� ������ �ٷ� ������ �ȵǴ°� ����. �׷��� 0.1�� ��ٸ��� ����
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
	DOREPLIFETIME(AParentCharacter, ChangeMesh);
	DOREPLIFETIME(AParentCharacter, ForcedMute);
}

void AParentCharacter::NamePlateUpdate()
{
	if (IsLocallyControlled()) NamePlate->SetVisibility(false);
	else if (GetLocalRole() != ROLE_Authority) NamePlate->SetVisibility(true);
	if (HasAuthority())
	{
		PlayerNameRef = this->GetPlayerState()->GetPlayerName();
		//OnRep_PlayerNameRef();
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

void AParentCharacter::OnRep_ChangeMesh()
{
	if(ChangeMesh != nullptr) GetMesh()->SetSkeletalMesh(ChangeMesh);
}

void AParentCharacter::VoiceChatOnOff()
{
	if (ForcedMute) return;

	if (IsVoiceChatOn)
	{
		UUniversalVoiceChat::VoiceChatStopSpeak();
		IsVoiceChatOn = false;
	}
	else
	{
		// �Ÿ� 2500�̸� �����ҵ�
		UUniversalVoiceChat::VoiceChatStartSpeak(true, true, 0, true, 1000);
		IsVoiceChatOn = true;
	}
}


void AParentCharacter::OnRep_ForcedMuteOnOff()
{
	if (ForcedMute)
	{
		IsVoiceChatOn = false;
		UUniversalVoiceChat::VoiceChatStopSpeak();
	}
}

void AParentCharacter::SetChangeMesh(USkeletalMesh* NewSkeletalMesh)
{
	ChangeMesh = NewSkeletalMesh;
}
