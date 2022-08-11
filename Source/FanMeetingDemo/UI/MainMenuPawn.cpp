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
	Pointer->SetupAttachment(GetRootComponent());

	ConstructorHelpers::FClassFinder<UMenuWidget> MainMenuUIBPClass(TEXT("/Game/UI/WBP_MainMenuUI"));
	if (MainMenuUIBPClass.Class == nullptr) return;
	MainMenu = MainMenuUIBPClass.Class;
}

void AMainMenuPawn::BeginPlay()
{
	Super::BeginPlay();
	
	// HMD를 사용하는지 여부
	UseHMD = UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayConnected()
		&& UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();

	if (!UseHMD)
	{
		if (MainMenu != nullptr)
		{
			UMenuWidget* MainMenuUI = CreateWidget<UMenuWidget>(GetWorld(), MainMenu);
			MainMenuUI->Setup();
		}
	}
	else
	{
		RightController->SetShowDeviceModel(true);
	}
}

void AMainMenuPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMainMenuPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("TriggerRight"), IE_Pressed, this, &AMainMenuPawn::TriggerRight);
}

void AMainMenuPawn::TriggerRight()
{
	Pointer->PressPointerKey(EKeys::LeftMouseButton);
}

