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

	void CalculateHMDToCharLocation();

	void HMDSyncLocation();

	UFUNCTION(Server, Unreliable)
		void Server_HMDSyncLocation(FVector NewLocation);

	//UPROPERTY(ReplicatedUsing = OnRep_RepLocation)
	//	FVector RepLocation;

	//UFUNCTION()
	//	void OnRep_RepLocation();

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	//Setting
	UPROPERTY(VisibleAnywhere, category = "Setting")
		float BaseTurnRate = 45.f;

	UPROPERTY(EditAnywhere, category = "Blinkers")
		bool UseBlinker = false;

	// Components
	UPROPERTY(VisibleAnywhere)
		class USceneComponent* VRRoot;

	UPROPERTY() // Blinker를 위한 포스트프로세스 컴포넌트
		class UPostProcessComponent* PostProcessComponent;

	// Property
	FVector HMDToCharLocation = FVector::ZeroVector;

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
