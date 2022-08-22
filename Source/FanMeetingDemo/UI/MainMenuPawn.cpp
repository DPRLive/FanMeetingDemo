#include "MainMenuPawn.h"
#include "HeadMountedDisplayFunctionLibrary.h"


AMainMenuPawn::AMainMenuPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<UMainMenuUI> MainMenuUIBPClass(TEXT("/Game/UI/WBP_LobbyUI"));
	if (MainMenuUIBPClass.Class == nullptr) return;
	MainMenu = MainMenuUIBPClass.Class;
}

void AMainMenuPawn::BeginPlay()
{
	Super::BeginPlay();
	UMenuWidget* MainMenuUI = CreateWidget<UMenuWidget>(GetWorld(), MainMenu);
	MainMenuUI->Setup();
}