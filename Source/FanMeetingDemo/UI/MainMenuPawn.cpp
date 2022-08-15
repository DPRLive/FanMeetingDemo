// Fill out your copyright notice in the Description page of Project Settings.

#include "HeadMountedDisplayFunctionLibrary.h"
#include "MainMenuPawn.h"

AMainMenuPawn::AMainMenuPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot"));
	SetRootComponent(VRRoot);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(VRRoot);

	RightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightController"));
	RightController->SetupAttachment(VRRoot);

	Pointer = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("Pointer"));
	Pointer->SetupAttachment(RightController);

	ConstructorHelpers::FClassFinder<UMainMenuUI> MainMenuUIBPClass(TEXT("/Game/UI/WBP_MainMenuUI"));
	if (MainMenuUIBPClass.Class == nullptr) return;
	MainMenu = MainMenuUIBPClass.Class;
}

void AMainMenuPawn::BeginPlay()
{
	Super::BeginPlay();
	
	// HMD�� ����ϴ��� ����
	UseHMD = UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayConnected()
		&& UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();

	// PC�� VR ���� ��Ű¡ �ؾ��ҰŰ��� ��
	if (!UseHMD)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not use hmd"));
		if (MainMenu != nullptr)
		{
			UMenuWidget* MainMenuUI = CreateWidget<UMenuWidget>(GetWorld(), MainMenu);
			MainMenuUI->Setup();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("use hmd"));
		RightController->SetShowDeviceModel(true);
		RightController->SetTrackingSource(EControllerHand::Right);
	}
}

void AMainMenuPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMainMenuPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("TriggerRight"), IE_Pressed, this, &AMainMenuPawn::TriggerRightPressed);
	PlayerInputComponent->BindAction(TEXT("TriggerRight"), IE_Released, this, &AMainMenuPawn::TriggerRightReleased);
}

void AMainMenuPawn::TriggerRightPressed()
{
	Pointer->PressPointerKey(EKeys::LeftMouseButton);
}

void AMainMenuPawn::TriggerRightReleased()
{
	Pointer->ReleasePointerKey(EKeys::LeftMouseButton);
}

