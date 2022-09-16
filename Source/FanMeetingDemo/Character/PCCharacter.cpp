// Fill out your copyright notice in the Description page of Project Settings.
#include "PCCharacter.h"
// custom header
#include "../UI/NamePlate.h"
#include "../FanMeetingPlayerState.h"
//plugin header
#include "UniversalVoiceChatPro/Public/PlayerVoiceChatActor.h"
// unreal header
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

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera")); // ī�޶� ������Ʈ
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

	FTimerHandle WaitHandle;
	// player state�� beginplay �ϴ� ������ �ٷ� ������ �ȵǴ°� ����. �׷��� 0.1�� ��ٸ��� ����
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			NamePlateUpdate();
		}), 0.1, false);
}

void APCCharacter::NamePlateUpdate()
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
		Cast<UNamePlate>(NamePlate->GetWidget())->SetPCCharacterRef(this);
	}
}

void APCCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APCCharacter, ControllerRotate);
	DOREPLIFETIME(APCCharacter, PlayerName);
	DOREPLIFETIME(APCCharacter, PlayerNameRef);
}

void APCCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// �Ӹ� ȸ��.. �������� �ǵ��� Ŭ���̾�Ʈ�� AnimBlueprint���� ����
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
}

void APCCharacter::OnRep_PlayerNameRef()
{
	PlayerName = PlayerNameRef;
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
	// Head Bone�� ���� ���� ���� 
	AController* MyController = GetController();
	if (MyController != nullptr)
	{
		float NowPitch = MyController->GetControlRotation().Pitch;
		if (NowPitch > 180 && NowPitch < 320 && Scale > 0)
		{
			return;
		}
		if (NowPitch < 180 && NowPitch > 90 && Scale < 0)
		{
			return;
		}
	}
	AddControllerPitchInput(Scale);
}

void APCCharacter::VoiceChatOnOff()
{
	if (IsVoiceChatOn)
	{
		StopSpeakGlobalVoiceChat();
		IsVoiceChatOn = false;
	}
	else
	{
		StartSpeakGlobalVoiceChat();
		IsVoiceChatOn = true;
	}
}

void APCCharacter::StartSpeakGlobalVoiceChat()
{
	// �ϴ� �� ��Ҹ� ���� �鸮��.
	UUniversalVoiceChat::VoiceChatStartSpeak(true, true, 0, true, 1000);
	UUniversalVoiceChat::VoiceChatSetMicrophoneVolume(10); //�ϴ� ���� 10. ���߿� UI�� ���� ����
}

void APCCharacter::StopSpeakGlobalVoiceChat()
{
	UUniversalVoiceChat::VoiceChatStopSpeak();
}
