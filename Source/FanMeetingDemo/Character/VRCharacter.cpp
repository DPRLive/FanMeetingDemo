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

	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot")); // vr 위치 조정을 위한 씬컴포넌트
	VRRoot->SetupAttachment(GetRootComponent());

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->SetupAttachment(VRRoot);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera")); // 카메라 컴포넌트
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
	// HMD를 사용하는지 여부
	UseHMD = UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayConnected()
		&& UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();

	VRRoot->AddLocalOffset(FVector(0, 0, -88));
	//cameraspringarm->addlocaloffset(fvector(charactereyeforward, 0, 0));
	//cameraspringarm->targetarmlength = 0;

	PostProcessComponent->bUnbound = false;

	if (UseHMD)
	{
		GetMesh()->SetAnimClass(VRAnimClass);
		// VR 사용시 세팅
		ControllerLeft->SetTrackingSource(EControllerHand::Left);
		ControllerRight->SetTrackingSource(EControllerHand::Right);
		//멀미 방지를 위한 Blinkers
		if (BlinkerMaterialBase != nullptr && PostProcessComponent != nullptr)
		{
			// 부모를 기준으로 하는 동적 Material 생성
			BlinkerMaterialInstance = UMaterialInstanceDynamic::Create(BlinkerMaterialBase, this);
			// 액터가 들고있는 PostProcess Component에 적용
			PostProcessComponent->AddOrUpdateBlendable(BlinkerMaterialInstance);
		}

	}
	else
	{
		// PC 사용시 세팅
		GetMesh()->SetAnimClass(PCAnimClass);
		Camera->AddLocalOffset(FVector(0, 0, 88));
	}
	// Super::BeginPlay를 나중에 호출해야 cpp코드가 블루프린트보다 먼저 호출됨.
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
	// 헤드마운트 디스플레이에 따른 캐릭터 위치 조정
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
	// BaseTurnRate 기반으로 회전한다.
	float NewYaw = GetControlRotation().Yaw + BaseTurnRate;
	GetController()->SetControlRotation(FRotator(0, NewYaw, 0));
}

void AVRCharacter::TurnLeftAction()
{
	float NewYaw = GetControlRotation().Yaw + (-BaseTurnRate);
	GetController()->SetControlRotation(FRotator(0, NewYaw, 0));
}
