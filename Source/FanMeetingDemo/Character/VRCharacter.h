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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
		void OnResetVR();

	//Blinkers
	UFUNCTION()
		void SetBlink();

	UFUNCTION(BlueprintCallable)
		bool GetUseBlinker() { return UseBlinker; }

	UFUNCTION(Server, Unreliable ,BlueprintCallable)
		void Server_ToggleIsSitting();

protected:
	// Components (BlueprintReadWrite)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UMotionControllerComponent* ControllerLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UMotionControllerComponent* ControllerRight;

	virtual void BeginPlay() override;

	UPROPERTY(Replicated, BlueprintReadOnly)
		bool IsSitting;

	//Menu
	UFUNCTION(BlueprintCallable)
		void MenuOnOff();

private:
	//Setting
	UPROPERTY(VisibleAnywhere, category = "Setting")
		float BaseTurnRate = 45.f;

	// Blinkers
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
	void UpdateBlinkers();

	//Movement
	virtual void MoveForward(float Scale) override;
	
	virtual void MoveRight(float Scale) override;

	UFUNCTION()
		void TurnRightAction();

	UFUNCTION()
		void TurnLeftAction();

	UPROPERTY(EditAnywhere)
		class UWidgetComponent* MenuComponent;

	UPROPERTY(VisibleAnywhere)
		class UWidgetInteractionComponent* RightPointer;

	UPROPERTY(VisibleAnywhere)
		class USplineMeshComponent* RightPointerMesh;

	UFUNCTION()
		void TriggerRightPressed();

	UFUNCTION()
		void TriggerRightReleased();
};
