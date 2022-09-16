// Fill out your copyright notice in the Description page of Project Settings.
#include "BasePawn.h"
//custom header
#include "../FanMeetingGameInstance.h"
#include "../FanMeetingPlayerState.h"

//unreal header
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"


ABasePawn::ABasePawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		UFanMeetingGameInstance* FMGameInstance = Cast<UFanMeetingGameInstance>(GetGameInstance());

		Cast<APlayerController>(GetController())->SetName(FMGameInstance->GetPlayerName());
		if (FMGameInstance->GetPlatformType() == 0) Server_SwapCharacter(this, 0);
		else if (FMGameInstance->GetPlatformType() == 1) Server_SwapCharacter(this, 1);
	}
}

void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("VRStart"), IE_Pressed, this, &ABasePawn::Test_VRStart);
	PlayerInputComponent->BindAction(TEXT("PCStart"), IE_Pressed, this, &ABasePawn::Test_PCStart);

}

void ABasePawn::Test_VRStart()
{
	Server_SwapCharacter(this, 1);
}

void ABasePawn::Test_PCStart()
{
	Server_SwapCharacter(this, 0);
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

