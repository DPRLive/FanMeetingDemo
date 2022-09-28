// Fill out your copyright notice in the Description page of Project Settings.
#include "PCCharacter.h"
// custom header
#include "../UI/InGameUI.h"
//plugin header
// unreal header
#include "Camera/PlayerCameraManager.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

APCCharacter::APCCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->bUsePawnControlRotation = true;
	CameraSpringArm->TargetArmLength = 0;
	CameraSpringArm->SetupAttachment(GetMesh());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera")); // 카메라 컴포넌트
	Camera->SetupAttachment(CameraSpringArm);

	NamePlate = CreateDefaultSubobject<UWidgetComponent>(TEXT("NamePlate"));
	NamePlate->SetDrawSize(FVector2D(300, 100));
	NamePlate->SetupAttachment(GetRootComponent());
}

void APCCharacter::BeginPlay()
{
	Super::BeginPlay();
	NamePlate->AddLocalOffset(FVector(0, 0, CharacterHeight / 2));
	CameraSpringArm->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("EyePosSocket"));
	
	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		APlayerCameraManager* PlayerCameraManager = Cast<APlayerController>(GetController())->PlayerCameraManager;
		if (PlayerCameraManager != nullptr)
		{
			PlayerCameraManager->ViewPitchMax = 90.0f;
			PlayerCameraManager->ViewPitchMin = -40.0f;
		}
	}
}

void APCCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APCCharacter, ControllerRotate);
}

void APCCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 머리 회전.. 서버에서 판독후 클라이언트의 AnimBlueprint에서 돌림
	if (HasAuthority())
	{
		AController* MyController = GetController();
		if (MyController != nullptr)
		{
			ControllerRotate = MyController->GetControlRotation();
		}
	}
}

void APCCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APCCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APCCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("TurnRightMouse"), this, &APCCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpMouse"), this, &APCCharacter::LookUpMouse);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &APCCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &APCCharacter::StopJumping);
	PlayerInputComponent->BindAction(TEXT("VoiceChatOnOff"), IE_Pressed, this, &APCCharacter::VoiceChatOnOff);
	PlayerInputComponent->BindAction(TEXT("MenuOnOff"), IE_Pressed, this, &APCCharacter::MenuOnOff);
}

void APCCharacter::MoveForward(float Scale)
{
	AddMovementInput(GetActorForwardVector(), Scale);
}

void APCCharacter::MoveRight(float Scale)
{
	AddMovementInput(GetActorRightVector(), Scale);
}

void APCCharacter::LookUpMouse(float Scale)
{
	AddControllerPitchInput(Scale);
}

void APCCharacter::MenuOnOff()
{
	if (InGameUI == nullptr)
	{
		if (MenuWidget == nullptr) return;
		InGameUI = CreateWidget<UMenuWidget>(GetWorld(), MenuWidget);
		InGameUI->SetOwner(this);
		InGameUI->SetOwnerPlatformType(0);
		if (InGameUI != nullptr && !InGameUI->IsSetup()) InGameUI->Setup();
	}
	else
	{
		if (InGameUI != nullptr && !InGameUI->IsSetup()) InGameUI->Setup();
		else InGameUI->Teardown();
	}
}

void APCCharacter::LockPlayerCameraYaw()
{
	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		APlayerCameraManager* PlayerCameraManager = Cast<APlayerController>(GetController())->PlayerCameraManager;
		if (PlayerCameraManager != nullptr)
		{
			PlayerCameraManager->ViewPitchMax = 20.0f;
			PlayerCameraManager->ViewPitchMin = -25.0f;
			PlayerCameraManager->ViewYawMax = 266.0f;
			PlayerCameraManager->ViewYawMin = 95.0f;
		}
	}
}

void APCCharacter::UnLockPlayerCameraYaw()
{
	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		APlayerCameraManager* PlayerCameraManager = Cast<APlayerController>(GetController())->PlayerCameraManager;
		if (PlayerCameraManager != nullptr)
		{
			PlayerCameraManager->ViewPitchMax = 90.0f;
			PlayerCameraManager->ViewPitchMin = -40.0f;
			PlayerCameraManager->ViewYawMax = 359.9f;
			PlayerCameraManager->ViewYawMin = 0.0f;
		}
	}
}