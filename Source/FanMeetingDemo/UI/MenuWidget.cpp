// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"

void UMenuWidget::Setup()
{
	bSetup = true;

	this->AddToViewport();
	this->bIsFocusable = true;
	//FInputModeUIOnly InputMode;
	FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(this->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	APlayerController* PlayerController;

	if (Owner == nullptr) PlayerController = GetWorld()->GetFirstPlayerController();
	else PlayerController = Cast<APlayerController>(Owner->GetController());

	if (!ensure(PlayerController != nullptr)) return;
	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = true;
}

void UMenuWidget::Teardown()
{
	bSetup = false;

	FInputModeGameOnly InputMode;

	APlayerController* PlayerController;
	if (Owner == nullptr) PlayerController = GetWorld()->GetFirstPlayerController();
	else PlayerController = Cast<APlayerController>(Owner->GetController());

	if (!ensure(PlayerController != nullptr)) return;
	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = false;

	this->bIsFocusable = false;
	this->RemoveFromViewport();
}