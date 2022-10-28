// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ParentCharacter.h"
#include "PCCharacter.generated.h"

UCLASS()
class FANMEETINGDEMO_API APCCharacter : public AParentCharacter
{
	GENERATED_BODY()

public:
	APCCharacter();

protected:
	virtual void BeginPlay() override;

	// Head bone �� �Ʒ� ȸ���� ����
	UPROPERTY(Replicated, BlueprintReadWrite)
		FRotator ControllerRotate;

	UFUNCTION(BlueprintCallable)
		void LockPlayerCameraYaw();

	UFUNCTION(BlueprintCallable)
		void UnLockPlayerCameraYaw();
public:	
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// setting
	UPROPERTY(EditAnywhere, category = "Setting")
		float CharacterHeight = 0; //NamePlate���߿� ���Ͽ� �߰��ϸ� ������

	// components
	UPROPERTY(EditAnywhere)
		class USpringArmComponent* CameraSpringArm;

	//Movement
	virtual void MoveForward(float Scale) override;

	virtual void MoveRight(float Scale) override;

	UFUNCTION()
		void LookUpMouse(float Scale);

	//Menu
	void MenuOnOff();
};
