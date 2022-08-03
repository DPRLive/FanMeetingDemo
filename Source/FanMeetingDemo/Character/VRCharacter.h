// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//custom header
#include "HandController.h"

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
	virtual void BeginPlay() override;

	void HMDSyncLocationAndRotation();

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	//Setting
	bool UseHMD;

	UPROPERTY(VisibleAnywhere, category = "Setting")
		float BaseTurnRate = 45.f;

	UPROPERTY(EditAnywhere, category = "Setting")
		float CharacterEyeHeight = 89;

	UPROPERTY(EditAnywhere, category = "Setting")
		float CharacterEyeForward = 40;

	UPROPERTY(EditAnywhere, category = "Blinkers")
		bool UseBlinker = true;

	// MotionController
	UPROPERTY()
		AHandController* LeftController;

	UPROPERTY()
		AHandController* RightController;

	// Components
	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
		class USceneComponent* VRRoot;

	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* CameraSpringArm;

	UPROPERTY() // Blinker를 위한 포스트프로세스 컴포넌트
		class UPostProcessComponent* PostProcessComponent;

	// Property
	UPROPERTY(EditDefaultsOnly, category = "Setting") //BP_HandController를 담는다.
		TSubclassOf<AHandController> HandControllerClass;

	UPROPERTY()
		class UMaterialInstanceDynamic* BlinkerMaterialInstance;

	UPROPERTY(EditAnywhere, category = "Blinkers")
		class UMaterialInterface* BlinkerMaterialBase;

	UPROPERTY(EditAnywhere, category = "Blinkers") //동적인 Blink를 위한 커브 
		class UCurveFloat* RadiusVsVelocity;

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
