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

	LeftController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftController"));
	LeftController->SetupAttachment(VRRoot);

	RightPointer = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("RightPointer"));
	RightPointer->SetupAttachment(RightController);

	LeftPointer = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("LeftPointer"));
	LeftPointer->SetupAttachment(LeftController);

	ConstructorHelpers::FClassFinder<UMainMenuUI> MainMenuUIBPClass(TEXT("/Game/UI/WBP_LobbyUI"));
	if (MainMenuUIBPClass.Class == nullptr) return;
	MainMenu = MainMenuUIBPClass.Class;
}

void AMainMenuPawn::BeginPlay()
{
	Super::BeginPlay();
	
	if (RightController != nullptr)
	{
		RightController->SetShowDeviceModel(true);
		RightController->SetTrackingSource(EControllerHand::Right);
	}

	if (LeftController != nullptr)
	{
		LeftController->SetShowDeviceModel(true);
		LeftController->SetTrackingSource(EControllerHand::Left);
	}

	// HMD를 사용하는지 여부
	//UseHMD = UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayConnected()
	//	&& UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();

	// PC와 VR 따로 패키징 해야할거같음 ㅠ
	//if (!UseHMD)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Not use hmd"));
	//	
	//}
	//else
	//{

	//}
}

void AMainMenuPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMainMenuPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("PCStart"), IE_Pressed, this, &AMainMenuPawn::PCStart);
	PlayerInputComponent->BindAction(TEXT("TriggerRight"), IE_Pressed, this, &AMainMenuPawn::TriggerRightPressed);
	PlayerInputComponent->BindAction(TEXT("TriggerRight"), IE_Released, this, &AMainMenuPawn::TriggerRightReleased);
	PlayerInputComponent->BindAction(TEXT("TriggerLeft"), IE_Pressed, this, &AMainMenuPawn::TriggerLeftPressed);
	PlayerInputComponent->BindAction(TEXT("TriggerLeft"), IE_Released, this, &AMainMenuPawn::TriggerLeftReleased);
}

void AMainMenuPawn::TriggerRightPressed()
{
	RightPointer->PressPointerKey(EKeys::LeftMouseButton);
}

void AMainMenuPawn::TriggerLeftPressed()
{
	LeftPointer->PressPointerKey(EKeys::LeftMouseButton);
}

void AMainMenuPawn::TriggerRightReleased()
{
	RightPointer->ReleasePointerKey(EKeys::LeftMouseButton);
}

void AMainMenuPawn::TriggerLeftReleased()
{
	LeftPointer->ReleasePointerKey(EKeys::LeftMouseButton);
}

void AMainMenuPawn::PCStart()
{
	if (MainMenu != nullptr)
	{
		UMenuWidget* MainMenuUI = CreateWidget<UMenuWidget>(GetWorld(), MainMenu);
		MainMenuUI->Setup();
	}
}

