// Fill out your copyright notice in the Description page of Project Settings.

//custom header


//unreal header
#include "HandController.h"

AHandController::AHandController()
{
	PrimaryActorTick.bCanEverTick = true;

	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
	SetRootComponent(MotionController);
}

void AHandController::SetHand(EControllerHand Hand)
{
	if (MotionController != nullptr)
	{
		MotionController->SetTrackingSource(Hand);
		// 알아서 컨트롤러에 맞춰 스태틱 메시 보이게함
		MotionController->bDisplayDeviceModel = true;
	}
}

void AHandController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHandController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

