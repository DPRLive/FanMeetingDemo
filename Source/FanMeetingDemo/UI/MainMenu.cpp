// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

AMainMenu::AMainMenu()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	MainMenuUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("MainMenuUI"));
	MainMenuUI->SetupAttachment(Root);
}

void AMainMenu::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMainMenu::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}