// Fill out your copyright notice in the Description page of Project Settings.
#include "BasePawn.h"
//custom header
#include "../FanMeetingGameInstance.h"
#include "../FanMeetingPlayerState.h"
#include "../Character/ParentCharacter.h"

//unreal header
#include "UObject/UObjectGlobals.h"
#include "Kismet/GameplayStatics.h"
#include "../FanMeetingPlayerState.h"
#include "GameFramework/Character.h"


ABasePawn::ABasePawn()
{
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<USkeletalMesh> FanMesh(TEXT("/Game/VTuberCharacter/fan"));
	if (FanMesh.Object != nullptr) FanMeshObj = FanMesh.Object;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> VTuberMesh(TEXT("/Game/VTuberCharacter/VTuberCharacter"));
	if (VTuberMesh.Object != nullptr) VTuberMeshObj = VTuberMesh.Object;
}

void ABasePawn::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		FTimerHandle WaitHandle;
		GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
			{
				AFanMeetingPlayerState* FMPlayerState = Cast<AFanMeetingPlayerState>(this->GetPlayerState());
				UFanMeetingGameInstance* FMGameInstance = Cast<UFanMeetingGameInstance>(GetGameInstance());

				Cast<APlayerController>(GetController())->SetName(FMGameInstance->GetPlayerName());
				int PlatformType = FMGameInstance->GetPlatformType();
				FString JoinType = FMGameInstance->GetJoinType();

				//�������� ������ �Ѱ��ش�
				if (FMPlayerState != nullptr)
				{
					FMPlayerState->Server_SetJoinType(JoinType);
				}
				Server_SwapCharacter(this, PlatformType, JoinType);
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
}

void ABasePawn::Server_SwapCharacter_Implementation(APawn* NowPawn, int PlatformType, const FString& JoinType)
{
	APlayerController* MyController = Cast<APlayerController>(NowPawn->GetController());
	FVector SpawnLocation = NowPawn->GetActorLocation();
	ACharacter* Character = nullptr;

	// 0 : PC , 1 : VR
	if (PlatformType == 0) // ������ ��Ÿ�޸�, �Ŵ��� ����ŷ, ȣ��Ʈ �𵨸� ĳ����
	{
		if (JoinType.Compare("MANAGER") == 0)
			Character = Cast<ACharacter>(GetWorld()->SpawnActor(N_PCCharacterClass, &SpawnLocation));
		else if (JoinType.Compare("VTUBER") == 0)
		{
			Character = Cast<ACharacter>(GetWorld()->SpawnActor(N_PCCharacterClass, &SpawnLocation));

			if(VTuberMeshObj != nullptr) Cast<AParentCharacter>(Character)->SetChangeMesh(VTuberMeshObj);
		}
		else if (JoinType.Compare("FAN") == 0)
		{
			Character = Cast<ACharacter>(GetWorld()->SpawnActor(N_PCCharacterClass, &SpawnLocation));
			if (FanMeshObj != nullptr) Cast<AParentCharacter>(Character)->SetChangeMesh(FanMeshObj);
		}
	}
	else if (PlatformType == 1)
	{
		if (JoinType.Compare("MANAGER") == 0)
			Character = Cast<ACharacter>(GetWorld()->SpawnActor(N_VRCharacterClass, &SpawnLocation));
		else if (JoinType.Compare("VTUBER") == 0)
		{
			Character = Cast<ACharacter>(GetWorld()->SpawnActor(VT_VRCharacterClass, &SpawnLocation));
		}
		else if (JoinType.Compare("FAN") == 0)
		{
			Character = Cast<ACharacter>(GetWorld()->SpawnActor(N_VRCharacterClass, &SpawnLocation));
			if (FanMeshObj != nullptr) Cast<AParentCharacter>(Character)->SetChangeMesh(FanMeshObj);

		}
	}

	MyController->UnPossess();
	MyController->Possess(Character);
	NowPawn->Destroy();

}