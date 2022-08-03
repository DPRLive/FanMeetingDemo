// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//custom header


//unreal header
#include "MotionControllerComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HandController.generated.h"

UCLASS()
class FANMEETINGDEMO_API AHandController : public AActor
{
	GENERATED_BODY()
	
public:	
	AHandController();

	void SetHand(EControllerHand Hand);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
		UMotionControllerComponent* MotionController;
};
