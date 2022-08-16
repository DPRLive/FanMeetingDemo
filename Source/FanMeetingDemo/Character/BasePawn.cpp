// Fill out your copyright notice in the Description page of Project Settings.

//custom header
#include "../FanMeetingGameInstance.h"
#include "../FanMeetingPlayerState.h"

//unreal header
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "BasePawn.h"

ABasePawn::ABasePawn()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		FTimerHandle WaitHandle;
		// player state가 beginplay 하는 시점에 바로 생성이 안되는거 같음. 그래서 0.1초 기다리고 접근
		GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
			{
				UFanMeetingGameInstance* FMGameInstance = Cast<UFanMeetingGameInstance>(GetGameInstance());
				AFanMeetingPlayerState* FMPlayerState = Cast<AFanMeetingPlayerState>(GetPlayerState());

				if (FMGameInstance != nullptr && FMPlayerState != nullptr)
				{
					FMPlayerState->SetPlayerName(FMGameInstance->GetPlayerName());
					FMPlayerState->SetPlatformType(FMGameInstance->GetPlatformType());
				}

				if (FMGameInstance->GetPlatformType() == 0) PCStart();
				else if (FMGameInstance->GetPlatformType() == 1) VRStart();
			}), 0.1, false);
	}
}

void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction(TEXT("PCStart"), IE_Pressed, this, &ABasePawn::PCStart);
	PlayerInputComponent->BindAction(TEXT("VRStart"), IE_Pressed, this, &ABasePawn::VRStart);
}

void ABasePawn::VRStart()
{
	if (VRCharacterClass != nullptr)
	{
		Server_SwapCharacter(this,1);
	}
}

void ABasePawn::PCStart()
{
	if (PCCharacterClass != nullptr)
	{
		Server_SwapCharacter(this, 0);
	}
}

void ABasePawn::Server_SwapCharacter_Implementation(APawn* NowPawn, int Type)
{
	APlayerController* MyController = Cast<APlayerController>(NowPawn->GetController());
	FVector SpawnLocation = NowPawn->GetActorLocation();
	ACharacter* Character = nullptr;
	if (Type == 1)
		Character = Cast<ACharacter>(GetWorld()->SpawnActor(VRCharacterClass, &SpawnLocation));
	else if (Type == 0)
		Character = Cast<ACharacter>(GetWorld()->SpawnActor(PCCharacterClass, &SpawnLocation));
	
	MyController->UnPossess();
	MyController->Possess(Character);
	NowPawn->Destroy();
}

