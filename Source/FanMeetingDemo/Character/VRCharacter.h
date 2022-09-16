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
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// NamePlates
	void NamePlateUpdate();

	UPROPERTY(Replicated, BlueprintReadWrite)
		FString PlayerName;

	UPROPERTY(ReplicatedUsing = OnRep_PlayerNameRef, BlueprintReadWrite)
		FString PlayerNameRef;

	UFUNCTION()
		void OnRep_PlayerNameRef();

	// Components (BlueprintReadWrite)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UMotionControllerComponent* ControllerLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UMotionControllerComponent* ControllerRight;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		bool GetIsVoiceChatOn() { return IsVoiceChatOn; }

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void VoiceChatOnOff();
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

	UPROPERTY(EditAnywhere)
		class UWidgetComponent* NamePlate;

	// Property
	//FVector HMDToCharLocation = FVector::ZeroVector;

	UPROPERTY()
		class UMaterialInstanceDynamic* BlinkerMaterialInstance;

	UPROPERTY(EditAnywhere, category = "Blinkers")
		class UMaterialInterface* BlinkerMaterialBase;

	UPROPERTY(EditAnywhere, category = "Blinkers") //동적인 Blink를 위한 커브 
		class UCurveFloat* RadiusVsVelocity;

	// Function
	UFUNCTION()
		void OnResetVR();

	UFUNCTION()
		void SetBlink();

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

	//Voice
	bool IsVoiceChatOn = false;
};
