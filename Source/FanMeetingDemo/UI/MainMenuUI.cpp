// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/GameplayStatics.h"
#include "MainMenuUI.h"

bool UMainMenuUI::Initialize()
{
	if (!Super::Initialize()) return false;

	if (PCBtn == nullptr || VRBtn == nullptr) return false;

	PCBtn->OnClicked.AddDynamic(this, &UMainMenuUI::PCBtnClicked);
	VRBtn->OnClicked.AddDynamic(this, &UMainMenuUI::VRBtnClicked);

	return true;
}

void UMainMenuUI::PCBtnClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Maps/FanMeetingMap"), true, "Platform=PC");
	Teardown();
}

void UMainMenuUI::VRBtnClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Maps/FanMeetingMap"), true, "Platform=VR");
	Teardown();
}
