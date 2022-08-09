// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Character.h"
#include "BasePawn.h"

ABasePawn::ABasePawn()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	
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
		Server_SwapCharacter(this, 0);
	}
}

void ABasePawn::PCStart()
{
	if (PCCharacterClass != nullptr)
	{
		Server_SwapCharacter(this, 1);
	}
}

void ABasePawn::Server_SwapCharacter_Implementation(APawn* NowPawn, int Type)
{
	APlayerController* MyController = Cast<APlayerController>(NowPawn->GetController());
	ACharacter* Character = nullptr;
	if (Type == 0)
		Character = Cast<ACharacter>(GetWorld()->SpawnActor(VRCharacterClass));
	else if (Type == 1)
		Character = Cast<ACharacter>(GetWorld()->SpawnActor(PCCharacterClass));
	
	MyController->UnPossess();
	MyController->Possess(Character);
	NowPawn->Destroy();
}

