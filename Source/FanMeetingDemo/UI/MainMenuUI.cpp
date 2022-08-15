// Fill out your copyright notice in the Description page of Project Settings.

// custom headers
#include "../FanMeetingGameInstance.h"

// unreal headers
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
	UFanMeetingGameInstance* GameInstance = Cast<UFanMeetingGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance == nullptr) return;
	
	GameInstance->SetPlayerName("PCMan");
	GameInstance->SetPlatformType(0);
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("127.0.0.1"));
	if (IsSetup()) Teardown();
	
}

void UMainMenuUI::VRBtnClicked()
{
	UFanMeetingGameInstance* GameInstance = Cast<UFanMeetingGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance == nullptr) return;
	
	GameInstance->SetPlayerName("VRMan");
	GameInstance->SetPlatformType(1);
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("127.0.0.1"));
	if (IsSetup()) Teardown();

}
