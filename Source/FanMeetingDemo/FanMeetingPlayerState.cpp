// Fill out your copyright notice in the Description page of Project Settings.

#include "FanMeetingPlayerState.h"

#include "Net/UnrealNetwork.h"


void AFanMeetingPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFanMeetingPlayerState, JoinType);
}

void AFanMeetingPlayerState::Server_SetJoinType_Implementation(const FString& S)
{
	JoinType = S;
}
