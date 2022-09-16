// Fill out your copyright notice in the Description page of Project Settings.
#include "MainMenuUI.h"
// custom headers
#include "../FanMeetingGameInstance.h"

// unreal headers
#include "Framework/Application/SlateApplication.h"
#include "Kismet/GameplayStatics.h"


bool UMainMenuUI::Initialize()
{
	if (!Super::Initialize()) return false;
	return true;
}

void UMainMenuUI::JoinLevel()
{
	UFanMeetingGameInstance* GameInstance = Cast<UFanMeetingGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance == nullptr) return;
	GameInstance->SetPlatformType(0); //0:pc 1:vr
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("127.0.0.1"));
	if (IsSetup()) Teardown();
}
