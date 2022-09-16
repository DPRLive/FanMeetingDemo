// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PCCharacter.generated.h"

UCLASS()
class FANMEETINGDEMO_API APCCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APCCharacter();

protected:
	virtual void BeginPlay() override;

	void NamePlateUpdate();

	// Head bone 위 아래 회전을 위함
	UPROPERTY(Replicated, BlueprintReadWrite)
		FRotator ControllerRotate;

	UPROPERTY(Replicated, BlueprintReadWrite)
		FString PlayerName;

	UPROPERTY(ReplicatedUsing = OnRep_PlayerNameRef, BlueprintReadWrite)
		FString PlayerNameRef;

	UFUNCTION()
		void OnRep_PlayerNameRef();

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	// for GetAllActorsOfClass
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AActor> PlayerControllerClass;

	// setting
	UPROPERTY(EditAnywhere, category = "Setting")
		float EyeForwardPosition = 0;
	
	UPROPERTY(EditAnywhere, category = "Setting")
		float CharacterHeight = 0;

	// components
	UPROPERTY(EditAnywhere)
		class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
		class USpringArmComponent* CameraSpringArm;

	UPROPERTY(EditAnywhere)
		class UWidgetComponent* NamePlate;

	//Movement
	UFUNCTION()
		void MoveForward(float Scale);

	UFUNCTION()
		void MoveRight(float Scale);

	UFUNCTION()
		void LookUpMouse(float Scale);

	//Voice
	bool IsVoiceChatOn = false;
	UFUNCTION()
		void VoiceChatOnOff();

	UFUNCTION()
		void StartSpeakGlobalVoiceChat();

	UFUNCTION()
		void StopSpeakGlobalVoiceChat();
};
