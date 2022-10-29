// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FanMeetingPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class FANMEETINGDEMO_API AFanMeetingPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UFUNCTION(Server, unreliable)
		void Server_SetJoinType(const FString& S);

	UFUNCTION(BlueprintCallable)
		FString& GetJoinType() { return JoinType; }
protected:
	UPROPERTY(Replicated)
		FString JoinType;

};
