// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//custom header

//unreal header
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VRCharacter.generated.h"

UCLASS()
class FANMEETINGDEMO_API AVRCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AVRCharacter();

protected:
	// Components (BlueprintReadWrite)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UMotionControllerComponent* ControllerLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UMotionControllerComponent* ControllerRight;

	virtual void BeginPlay() override;

	void HMDSyncLocation();

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	//Setting
	bool UseHMD;

	UPROPERTY(VisibleAnywhere, category = "Setting")
		float BaseTurnRate = 45.f;

	UPROPERTY(EditAnywhere, category = "Setting")
		float CharacterEyeHeight = 75;

	UPROPERTY(EditAnywhere, category = "Setting")
		float CharacterEyeForward = 40;

	UPROPERTY(EditAnywhere, category = "Blinkers")
		bool UseBlinker = false;

	// Components
	UPROPERTY(VisibleAnywhere)
		class USceneComponent* VRRoot;

	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* CameraSpringArm;

	UPROPERTY() // Blinker를 위한 포스트프로세스 컴포넌트
		class UPostProcessComponent* PostProcessComponent;

	// Property
	UPROPERTY()
		class UMaterialInstanceDynamic* BlinkerMaterialInstance;

	UPROPERTY(EditAnywhere, category = "Blinkers")
		class UMaterialInterface* BlinkerMaterialBase;

	UPROPERTY(EditAnywhere, category = "Blinkers") //동적인 Blink를 위한 커브 
		class UCurveFloat* RadiusVsVelocity;

	UPROPERTY(EditDefaultsOnly, category = "Setting")
		TSubclassOf<class UAnimInstance> PCAnimClass;

	UPROPERTY(EditDefaultsOnly, category = "Setting")
		TSubclassOf<class UAnimInstance> VRAnimClass;

	// Function
	UFUNCTION()
		void OnResetVR();

	void UpdateBlinkers();

	//Movement
	UFUNCTION()
		void MoveForward(float Scale);
	
	UFUNCTION()
		void MoveRight(float Scale);

	UFUNCTION()
		void TurnRightAction();

	UFUNCTION()
		void TurnLeftAction();
};
