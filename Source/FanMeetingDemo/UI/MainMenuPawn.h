// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MainMenuUI.h"
#include "Components/WidgetInteractionComponent.h"
#include "MotionControllerComponent.h"
#include "Camera/CameraComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainMenuPawn.generated.h"

UCLASS()
class FANMEETINGDEMO_API AMainMenuPawn : public APawn
{
	GENERATED_BODY()

public:
	AMainMenuPawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere)
		USceneComponent* VRRoot;

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
		UMotionControllerComponent* RightController;

	UPROPERTY(VisibleAnywhere)
		UMotionControllerComponent* LeftController;

	UPROPERTY(VisibleAnywhere)
		UWidgetInteractionComponent* RightPointer;

	UPROPERTY(VisibleAnywhere)
		UWidgetInteractionComponent* LeftPointer;

	UPROPERTY()
		TSubclassOf<UMainMenuUI> MainMenu;

	UFUNCTION()
		void TriggerRightPressed();

	UFUNCTION()
		void TriggerLeftPressed();

	UFUNCTION()
		void TriggerRightReleased();

	UFUNCTION()
		void TriggerLeftReleased();

	UFUNCTION()
		void Test_PCStart();

};
