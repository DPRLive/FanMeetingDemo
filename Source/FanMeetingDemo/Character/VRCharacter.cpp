// Fill out your copyright notice in the Description page of Project Settings.

//custom header


//unreal header
#include "Animation/AnimInstance.h"
#include "MotionControllerComponent.h"
#include "Components/PostProcessComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "VRCharacter.h"

AVRCharacter::AVRCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = true;

	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot")); // vr ��ġ ������ ���� ��������Ʈ
	VRRoot->SetupAttachment(GetRootComponent());

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->SetupAttachment(VRRoot);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera")); // ī�޶� ������Ʈ
	Camera->SetupAttachment(CameraSpringArm);

	ControllerLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("ControllerLeft"));
	ControllerLeft->SetupAttachment(VRRoot);

	ControllerRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("ControllerRight"));
	ControllerRight->SetupAttachment(VRRoot);

	PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
	PostProcessComponent->SetupAttachment(GetRootComponent());
}

void AVRCharacter::BeginPlay()
{
	// HMD�� ����ϴ��� ����
	UseHMD = UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayConnected()
		&& UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();

	VRRoot->AddLocalOffset(FVector(0, 0, -88));
	//cameraspringarm->addlocaloffset(fvector(charactereyeforward, 0, 0));
	//cameraspringarm->targetarmlength = 0;

	PostProcessComponent->bUnbound = false;

	if (UseHMD)
	{
		GetMesh()->SetAnimClass(VRAnimClass);
		// VR ���� ����
		ControllerLeft->SetTrackingSource(EControllerHand::Left);
		ControllerRight->SetTrackingSource(EControllerHand::Right);
		//�ֹ� ������ ���� Blinkers
		if (BlinkerMaterialBase != nullptr && PostProcessComponent != nullptr)
		{
			// �θ� �������� �ϴ� ���� Material ����
			BlinkerMaterialInstance = UMaterialInstanceDynamic::Create(BlinkerMaterialBase, this);
			// ���Ͱ� ����ִ� PostProcess Component�� ����
			PostProcessComponent->AddOrUpdateBlendable(BlinkerMaterialInstance);
		}

	}
	else
	{
		// PC ���� ����
		GetMesh()->SetAnimClass(PCAnimClass);
		Camera->AddLocalOffset(FVector(0, 0, 88));
	}
	// Super::BeginPlay�� ���߿� ȣ���ؾ� cpp�ڵ尡 �������Ʈ���� ���� ȣ���.
	Super::BeginPlay();
}

void AVRCharacter::Tick(float DeltaTime)
{
	if (UseHMD)
	{
		//HMDSyncLocation();
		if (UseBlinker) UpdateBlinkers();
	}
	Super::Tick(DeltaTime);
}

void AVRCharacter::HMDSyncLocation()
{
	// ��帶��Ʈ ���÷��̿� ���� ĳ���� ��ġ ����
	FVector NewCameraOffset = Camera->GetComponentLocation() - GetActorLocation();
	NewCameraOffset.Z = 0;
	AddActorWorldOffset(NewCameraOffset);
	VRRoot->AddWorldOffset(-NewCameraOffset);
}

void AVRCharacter::UpdateBlinkers()
{
	if (RadiusVsVelocity == nullptr) return;
	float Speed = GetVelocity().Size();
	float Radius = RadiusVsVelocity->GetFloatValue(Speed);

	BlinkerMaterialInstance->SetScalarParameterValue(TEXT("Radius"), Radius);
	BlinkerMaterialInstance->SetVectorParameterValue(TEXT("Centre"), FLinearColor(0.5, 0.5, 0));
}

void AVRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Movement
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AVRCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AVRCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("TurnRightMouse"), this, &AVRCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpMouse"), this, &AVRCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction(TEXT("TurnRightAction"), IE_Pressed, this, &AVRCharacter::TurnRightAction);
	PlayerInputComponent->BindAction(TEXT("TurnLeftAction"), IE_Pressed, this, &AVRCharacter::TurnLeftAction);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AVRCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &AVRCharacter::StopJumping);
	PlayerInputComponent->BindAction(TEXT("OnResetVR"), IE_Pressed, this, &AVRCharacter::OnResetVR);
}

void AVRCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AVRCharacter::MoveForward(float Scale)
{
	AddMovementInput(Camera->GetForwardVector(), Scale);
}

void AVRCharacter::MoveRight(float Scale)
{
	AddMovementInput(Camera->GetRightVector(), Scale);
}

void AVRCharacter::TurnRightAction()
{
	// BaseTurnRate ������� ȸ���Ѵ�.
	float NewYaw = GetControlRotation().Yaw + BaseTurnRate;
	GetController()->SetControlRotation(FRotator(0, NewYaw, 0));
}

void AVRCharacter::TurnLeftAction()
{
	float NewYaw = GetControlRotation().Yaw + (-BaseTurnRate);
	GetController()->SetControlRotation(FRotator(0, NewYaw, 0));
}
