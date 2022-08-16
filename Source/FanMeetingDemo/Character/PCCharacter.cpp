// Fill out your copyright notice in the Description page of Project Settings.

// custom header
#include "../UI/NamePlate.h"
#include "../FanMeetingPlayerState.h"
// unreal header
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
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
	NamePlate->AddLocalOffset(FVector(0, 0, (CharacterHeight / 2)) + 20);

	FTimerHandle WaitHandle;
	// player state가 beginplay 하는 시점에 바로 생성이 안되는거 같음. 그래서 0.1초 기다리고 접근
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
		TArray<AActor*> ActorArray;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), PlayerControllerClass, ActorArray);
		for (int32 i = 0; i < ActorArray.Num(); i++)
		{
			PlayerNameRef = Cast<APlayerController>(ActorArray[i])->PlayerState->GetPlayerName();
			OnRep_PlayerNameRef();
		}
	}
	else
	{
		Cast<UNamePlate>(NamePlate->GetWidget())->SetPCCharacterRef(this);
	}
}

void APCCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APCCharacter, PlayerName);
	DOREPLIFETIME(APCCharacter, PlayerNameRef);
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

void APCCharacter::OnRep_PlayerNameRef()
{
	PlayerName = PlayerNameRef;
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
