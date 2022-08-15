// Fill out your copyright notice in the Description page of Project Settings.

#include "Net/UnrealNetwork.h"
#include "FanMeetingPlayerState.h"

void AFanMeetingPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFanMeetingPlayerState, PlatformType);
}

void AFanMeetingPlayerState::SetPlatformType(const uint8& I)
{
	PlatformType = I;
}

uint8 AFanMeetingPlayerState::GetPlatformType() const
{
	return PlatformType;
}
