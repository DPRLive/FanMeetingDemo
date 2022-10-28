// Fill out your copyright notice in the Description page of Project Settings.
#include "VRCharacter.h"
//custom header
#include "../UI/InGameUI.h"
//plugin header
//unreal header
#include "Components/SplineMeshComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/WidgetComponent.h"
#include "MotionControllerComponent.h"
#include "Components/PostProcessComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"


AVRCharacter::AVRCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot")); // vr 위치 조정을 위한 씬컴포넌트
	VRRoot->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera")); // 카메라 컴포넌트
	Camera->SetupAttachment(VRRoot);

	ControllerLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("ControllerLeft"));
	ControllerLeft->SetupAttachment(VRRoot);

	MenuComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("MenuComponent"));
	MenuComponent->SetupAttachment(ControllerLeft);
	MenuComponent->SetDrawSize(FVector2D(800, 800));

	ControllerRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("ControllerRight"));
	ControllerRight->SetupAttachment(VRRoot);

	RightPointer = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("RightPointer"));
	RightPointer->SetupAttachment(ControllerRight);

	RightPointerMesh = CreateDefaultSubobject<USplineMeshComponent>(TEXT("RightPointerMesh"));
	RightPointerMesh->SetupAttachment(RightPointer);

	PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
	PostProcessComponent->bUnbound = false;
	PostProcessComponent->SetupAttachment(GetRootComponent());

	NamePlate = CreateDefaultSubobject<UWidgetComponent>(TEXT("NamePlate"));
	NamePlate->SetDrawSize(FVector2D(300, 100));
	NamePlate->SetupAttachment(Camera);
}

void AVRCharacter::BeginPlay()
{
	VRRoot->AddLocalOffset(FVector(0, 0, -88));
	NamePlate->AddLocalOffset(FVector(0, 0, 20));
	ControllerLeft->SetTrackingSource(EControllerHand::Left);
	ControllerRight->SetTrackingSource(EControllerHand::Right);

	if (BlinkerMaterialBase != nullptr && PostProcessComponent != nullptr)
	{
		// 부모를 기준으로 하는 동적 Material 생성
		BlinkerMaterialInstance = UMaterialInstanceDynamic::Create(BlinkerMaterialBase, this);
		// 액터가 들고있는 PostProcess Component에 적용
		PostProcessComponent->AddOrUpdateBlendable(BlinkerMaterialInstance);
	}	
	// Super::BeginPlay를 나중에 호출해야 cpp코드가 블루프린트보다 먼저 호출됨.
	Super::BeginPlay();
	OnResetVR();
}

void AVRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (UseBlinker) UpdateBlinkers();
}

void AVRCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AVRCharacter, IsSitting);
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

	PlayerInputComponent->BindAction(TEXT("TurnRightAction"), IE_Pressed, this, &AVRCharacter::TurnRightAction);
	PlayerInputComponent->BindAction(TEXT("TurnLeftAction"), IE_Pressed, this, &AVRCharacter::TurnLeftAction);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AVRCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &AVRCharacter::StopJumping);
	PlayerInputComponent->BindAction(TEXT("OnResetVR"), IE_Pressed, this, &AVRCharacter::OnResetVR);
	//PlayerInputComponent->BindAction(TEXT("SetBlink"), IE_Pressed, this, &AVRCharacter::SetBlink);
	//PlayerInputComponent->BindAction(TEXT("VoiceChatOnOff"), IE_Pressed, this, &AVRCharacter::VoiceChatOnOff);
	PlayerInputComponent->BindAction(TEXT("MenuOnOff"), IE_Pressed, this, &AVRCharacter::MenuOnOff);
	PlayerInputComponent->BindAction(TEXT("TriggerRight"), IE_Pressed, this, &AVRCharacter::TriggerRightPressed);
	PlayerInputComponent->BindAction(TEXT("TriggerRight"), IE_Released, this, &AVRCharacter::TriggerRightReleased);
}

void AVRCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AVRCharacter::SetBlink()
{
	if (UseBlinker) UseBlinker = false;
	else UseBlinker = true;
}

void AVRCharacter::Server_ToggleIsSitting_Implementation()
{
	if (HasAuthority())
	{
		if (IsSitting) IsSitting = false;
		else IsSitting = true;
	}
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

void AVRCharacter::MenuOnOff()
{
	if (InGameUI == nullptr)
	{
		if (MenuWidget == nullptr) return;
		InGameUI = CreateWidget<UMenuWidget>(GetWorld(), MenuWidget);
		InGameUI->SetOwner(this);
		InGameUI->SetOwnerPlatformType(1);
		if (InGameUI != nullptr && MenuComponent != nullptr && MenuComponent->GetWidget() == nullptr)
		{
			RightPointerMesh->SetVisibility(true);
			ControllerLeft->SetShowDeviceModel(true);
			ControllerRight->SetShowDeviceModel(true);
			MenuComponent->SetWidget(InGameUI);
		}
	}
	else
	{
		if (InGameUI != nullptr && MenuComponent->GetWidget() == nullptr)
		{
			RightPointerMesh->SetVisibility(true);
			ControllerLeft->SetShowDeviceModel(true);
			ControllerRight->SetShowDeviceModel(true);
			MenuComponent->SetWidget(InGameUI);
		}
		else
		{
			if (MenuComponent == nullptr) return;
			RightPointerMesh->SetVisibility(false);
			ControllerLeft->SetShowDeviceModel(false);
			ControllerRight->SetShowDeviceModel(false);
			MenuComponent->SetWidget(nullptr);
		}
	}
}

void AVRCharacter::TriggerRightPressed()
{
	RightPointer->PressPointerKey(EKeys::LeftMouseButton);
}

void AVRCharacter::TriggerRightReleased()
{
	RightPointer->ReleasePointerKey(EKeys::LeftMouseButton);
}