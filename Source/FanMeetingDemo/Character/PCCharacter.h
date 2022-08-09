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

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
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

	//Movement
	UFUNCTION()
		void MoveForward(float Scale);

	UFUNCTION()
		void MoveRight(float Scale);
};
