// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MenuWidget.h"
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
	bool UseHMD;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* VRRoot;

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
		UMotionControllerComponent* RightController;

	UPROPERTY(VisibleAnywhere)
		UWidgetInteractionComponent* Pointer;

	TSubclassOf<UMenuWidget> MainMenu;

	void TriggerRight();
};
