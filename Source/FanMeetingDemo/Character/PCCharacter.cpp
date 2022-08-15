// Fill out your copyright notice in the Description page of Project Settings.

// custom header
#include "../UI/NamePlate.h"
#include "../FanMeetingPlayerState.h"
// unreal header
#include "Components/WidgetComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PCCharacter.h"


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
	NamePlate->SetDrawSize(FVector2D(500, 80));
	NamePlate->SetupAttachment(GetRootComponent());
}

void APCCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CameraSpringArm->SetRelativeLocation(FVector(0, EyeForwardPosition, CharacterHeight));
	NamePlate->AddLocalOffset(FVector(0, 0, CharacterHeight / 2));

	// TODO: other Player State 접근하는방법 알아야할듯.
	if (GetLocalRole() == ROLE_AutonomousProxy || GetLocalRole() == ROLE_SimulatedProxy)
	{
		FTimerHandle WaitHandle;
		GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
			{
				AFanMeetingPlayerState* FMPlayerState = Cast<AFanMeetingPlayerState>(GetPlayerState());

				if (FMPlayerState != nullptr)
				{
					FString PlayerName = FMPlayerState->GetPlayerName();
					Cast<UNamePlate>(NamePlate->GetWidget())->SetName(PlayerName);
				}

			}), 0.1, false);
	}
}

void APCCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APCCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APCCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APCCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("TurnRightMouse"), this, &APCCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpMouse"), this, &APCCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &APCCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &APCCharacter::StopJumping);
}

void APCCharacter::MoveForward(float Scale)
{
	AddMovementInput(GetActorForwardVector(), Scale);
	AFanMeetingPlayerState* FanPlayerState = Cast<AFanMeetingPlayerState>(GetPlayerState());
	//if (GetLocalRole() == ROLE_AutonomousProxy && FanPlayerState != nullptr)
		//UE_LOG(LogTemp, Warning, TEXT("Name : %s, Platform : %d"), *(FanPlayerState->GetPlayerName()), FanPlayerState->GetPlatformType());
	//if(GetLocalRole() == ROLE_AutonomousProxy && NamePlate != nullptr)
	//	Cast<UNamePlate>(NamePlate->GetWidget())->SetName("gelll");
}

void APCCharacter::MoveRight(float Scale)
{
	AddMovementInput(GetActorRightVector(), Scale);
}
