// Fill out your copyright notice in the Description page of Project Settings.
#include "VRCharacter.h"
//custom header
#include "../UI/NamePlate.h"
#include "../FanMeetingPlayerState.h"
//plugin header
#include "UniversalVoiceChatPro/Public/PlayerVoiceChatActor.h"
//unreal header
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

	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot")); // vr ��ġ ������ ���� ��������Ʈ
	VRRoot->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera")); // ī�޶� ������Ʈ
	Camera->SetupAttachment(VRRoot);

	ControllerLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("ControllerLeft"));
	ControllerLeft->SetupAttachment(VRRoot);

	ControllerRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("ControllerRight"));
	ControllerRight->SetupAttachment(VRRoot);

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
		// �θ� �������� �ϴ� ���� Material ����
		BlinkerMaterialInstance = UMaterialInstanceDynamic::Create(BlinkerMaterialBase, this);
		// ���Ͱ� ����ִ� PostProcess Component�� ����
		PostProcessComponent->AddOrUpdateBlendable(BlinkerMaterialInstance);
	}	
	// Super::BeginPlay�� ���߿� ȣ���ؾ� cpp�ڵ尡 �������Ʈ���� ���� ȣ���.
	Super::BeginPlay();
	OnResetVR();

	FTimerHandle WaitHandle;
	// player state�� beginplay �ϴ� ������ �ٷ� ������ �ȵǴ°� ����. �׷��� 0.1�� ��ٸ��� ����
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			NamePlateUpdate();
		}), 0.1, false);
}

void AVRCharacter::NamePlateUpdate()
{
	if (IsLocallyControlled()) NamePlate->SetVisibility(false);
	else if (GetLocalRole() != ROLE_Authority) NamePlate->SetVisibility(true);
	if (HasAuthority())
	{
		PlayerNameRef = this->GetPlayerState()->GetPlayerName();
		OnRep_PlayerNameRef();
	}
	else
	{
		Cast<UNamePlate>(NamePlate->GetWidget())->SetVRCharacterRef(this);
	}
}

void AVRCharacter::OnRep_PlayerNameRef()
{
	PlayerName = PlayerNameRef;
}

void AVRCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVRCharacter, PlayerName);
	DOREPLIFETIME(AVRCharacter, PlayerNameRef);
}

void AVRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (UseBlinker) UpdateBlinkers();
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
	PlayerInputComponent->BindAction(TEXT("SetBlink"), IE_Pressed, this, &AVRCharacter::SetBlink);
	PlayerInputComponent->BindAction(TEXT("VoiceChatOnOff"), IE_Pressed, this, &AVRCharacter::VoiceChatOnOff);

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

void AVRCharacter::VoiceChatOnOff()
{
	if (IsVoiceChatOn)
	{
		UUniversalVoiceChat::VoiceChatStopSpeak();
		IsVoiceChatOn = false;
	}
	else
	{
		UUniversalVoiceChat::VoiceChatStartSpeak(true, true, 0, true, 1000);
		IsVoiceChatOn = true;
	}
}

