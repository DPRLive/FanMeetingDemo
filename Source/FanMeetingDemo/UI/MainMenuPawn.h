#pragma once

#include "MainMenuUI.h"
#include "Components/WidgetInteractionComponent.h"
#include "MotionControllerComponent.h"
#include "Camera/CameraComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainMenuPawn.generated.h"


UCLASS()
class FANMEETINGDEMO_API AMainMenuPawn : public APawn
{
	GENERATED_BODY()

public:
	AMainMenuPawn();

protected:
	virtual void BeginPlay() override;

private:
	TSubclassOf<UMainMenuUI> MainMenu;
};