// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

UCLASS()
class FANMEETINGDEMO_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	ABasePawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ACharacter> MH_PCCharacterClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ACharacter> N_PCCharacterClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ACharacter> MH_VRCharacterClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ACharacter> N_VRCharacterClass;

	UFUNCTION(Server, Unreliable) // 0 = VR, 1 = PC
		void Server_SwapCharacter(APawn* NowPawn, int PlatformType, const FString& JoinType);

	UFUNCTION()
		void Test_VRStart();

	UFUNCTION()
		void Test_PCStart();
};
