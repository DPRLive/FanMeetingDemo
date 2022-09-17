// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//custom header

//unreal header
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "CoreMinimal.h"
#include "ParentCharacter.h"
#include "VRCharacter.generated.h"

UCLASS()
class FANMEETINGDEMO_API AVRCharacter : public AParentCharacter
{
	GENERATED_BODY()

public:
	AVRCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	// Components (BlueprintReadWrite)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UMotionControllerComponent* ControllerLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UMotionControllerComponent* ControllerRight;

	virtual void BeginPlay() override;


private:
	//Setting
	UPROPERTY(VisibleAnywhere, category = "Setting")
		float BaseTurnRate = 45.f;

	UPROPERTY(EditAnywhere, category = "Blinkers")
		bool UseBlinker = false;

	// Components
	UPROPERTY(VisibleAnywhere)
		class USceneComponent* VRRoot;

	UPROPERTY() // Blinker�� ���� ����Ʈ���μ��� ������Ʈ
		class UPostProcessComponent* PostProcessComponent;

	UPROPERTY()
		class UMaterialInstanceDynamic* BlinkerMaterialInstance;

	UPROPERTY(EditAnywhere, category = "Blinkers")
		class UMaterialInterface* BlinkerMaterialBase;

	UPROPERTY(EditAnywhere, category = "Blinkers") //������ Blink�� ���� Ŀ�� 
		class UCurveFloat* RadiusVsVelocity;

	// VRFunction
	UFUNCTION()
		void OnResetVR();

	UFUNCTION()
		void SetBlink();

	void UpdateBlinkers();

	//Movement
	virtual void MoveForward(float Scale) override;
	
	virtual void MoveRight(float Scale) override;

	UFUNCTION()
		void TurnRightAction();

	UFUNCTION()
		void TurnLeftAction();

	//Menu
	virtual void MenuOnOff() override;
};
