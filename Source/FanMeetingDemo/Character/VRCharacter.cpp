// Fill out your copyright notice in the Description page of Project Settings.

//custom header


//unreal header
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

	PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
	PostProcessComponent->SetupAttachment(GetRootComponent());
}

void AVRCharacter::BeginPlay()
{
	Super::BeginPlay();

	// HMD�� ����ϴ��� ����
	UseHMD = UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayConnected()
		&& UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();

	// ĳ������ �����̷� ī�޶� ��ġ ���� 
	VRRoot->AddLocalOffset(FVector(0, 0, CharacterEyeHeight));
	CameraSpringArm->AddLocalOffset(FVector(CharacterEyeForward, 0, 0));

	PostProcessComponent->bUnbound = false;

	if (UseHMD)
	{
		//�ֹ� ������ ���� Blinkers
		if (BlinkerMaterialBase != nullptr && PostProcessComponent != nullptr)
		{
			// �θ� �������� �ϴ� ���� Material ����
			BlinkerMaterialInstance = UMaterialInstanceDynamic::Create(BlinkerMaterialBase, this);
			// ���Ͱ� ����ִ� PostProcess Component�� ����
			PostProcessComponent->AddOrUpdateBlendable(BlinkerMaterialInstance);
		}
	
		//BP_HandController�� ĳ�����Ͽ� �����. �޼� ��Ʈ�ѷ�, ������ ��Ʈ�ѷ� ����.
		LeftController = GetWorld()->SpawnActor<AHandController>(HandControllerClass);
		if (LeftController != nullptr)
		{
			LeftController->AttachToComponent(VRRoot, FAttachmentTransformRules::KeepRelativeTransform);
			LeftController->SetHand(EControllerHand::Left);
			LeftController->SetOwner(this);
		}

		RightController = GetWorld()->SpawnActor<AHandController>(HandControllerClass);
		if (RightController != nullptr)
		{
			RightController->AttachToComponent(VRRoot, FAttachmentTransformRules::KeepRelativeTransform);
			RightController->SetHand(EControllerHand::Right);
			RightController->SetOwner(this);
		}
	}
}

void AVRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (UseHMD)
	{
		HMDSyncLocationAndRotation();
		if (UseBlinker) UpdateBlinkers();
	}
}

void AVRCharacter::HMDSyncLocationAndRotation()
{
	// ��帶��Ʈ ���÷��̿� ���� ĳ���� ��ġ ����
	FVector NewCameraOffset = Camera->GetComponentLocation() - GetActorLocation();
	NewCameraOffset.Z = 0;
	AddActorWorldOffset(NewCameraOffset);
	VRRoot->AddWorldOffset(-NewCameraOffset);

	// HMD ȸ���� ���� ��, �� ĳ���� ȸ��
	FRotator HMDRotation;
	FVector HMDPosition;
	UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(HMDRotation, HMDPosition);
	AddActorLocalRotation(FRotator(0, HMDRotation.Yaw, 0));
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
	AddMovementInput(GetActorForwardVector(), Scale);
}

void AVRCharacter::MoveRight(float Scale)
{
	AddMovementInput(GetActorRightVector(), Scale);
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
