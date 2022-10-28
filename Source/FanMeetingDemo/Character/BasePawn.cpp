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

				//서버에도 정보를 넘겨준다
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
	PlayerInputComponent->BindAction(TEXT("VRStart"), IE_Pressed, this, &ABasePawn::Test_VRStart);
	PlayerInputComponent->BindAction(TEXT("PCStart"), IE_Pressed, this, &ABasePawn::Test_PCStart);
}

void ABasePawn::Test_VRStart()
{
	Server_SwapCharacter(this, 1, TEXT("MANAGER"));
}

void ABasePawn::Test_PCStart()
{
	Server_SwapCharacter(this, 0, "FAN");
}

void ABasePawn::Server_SwapCharacter_Implementation(APawn* NowPawn, int PlatformType, const FString& JoinType)
{
	APlayerController* MyController = Cast<APlayerController>(NowPawn->GetController());
	FVector SpawnLocation = NowPawn->GetActorLocation();
	ACharacter* Character = nullptr;

	// 0 : PC , 1 : VR
	if (PlatformType == 0) // 참여자 메타휴먼, 매니저 마네킹, 호스트 모델링 캐릭터
	{
		if (JoinType.Compare("MANAGER") == 0)
			Character = Cast<ACharacter>(GetWorld()->SpawnActor(N_PCCharacterClass, &SpawnLocation));
		else if (JoinType.Compare("VTUBER") == 0)
		{
			Character = Cast<ACharacter>(GetWorld()->SpawnActor(N_PCCharacterClass, &SpawnLocation));

			USkeletalMesh* MySkeletalMesh = LoadObject<USkeletalMesh>(NULL, TEXT("SkeletalMesh'/Game/VTuberCharacter/VTuberCharacter.VTuberCharacter'"), NULL, LOAD_None, NULL);
			if (MySkeletalMesh != nullptr)
			{
				// SetChangeMesh를 통해 server -> client로 SkeletalMesh Replicated
				Cast<AParentCharacter>(Character)->SetChangeMesh(MySkeletalMesh);
			}
		}
		else if (JoinType.Compare("FAN") == 0)
			Character = Cast<ACharacter>(GetWorld()->SpawnActor(MH_PCCharacterClass, &SpawnLocation));
	}
	else if (PlatformType == 1)
	{
		if (JoinType.Compare("MANAGER") == 0)
			Character = Cast<ACharacter>(GetWorld()->SpawnActor(N_VRCharacterClass, &SpawnLocation));
		else if (JoinType.Compare("VTUBER") == 0)
		{
			Character = Cast<ACharacter>(GetWorld()->SpawnActor(VT_VRCharacterClass, &SpawnLocation));

			//USkeletalMesh* MySkeletalMesh = LoadObject<USkeletalMesh>(NULL, TEXT("SkeletalMesh'/Game/VTuberCharacter/VTuberCharacter.VTuberCharacter'"), NULL, LOAD_None, NULL);
			//if (MySkeletalMesh != nullptr)
			//{
			//	// SetChangeMesh를 통해 server -> client로 SkeletalMesh Replicated
			//	Cast<AParentCharacter>(Character)->SetChangeMesh(MySkeletalMesh);
			//}
		}
		else if (JoinType.Compare("FAN") == 0)
			Character = Cast<ACharacter>(GetWorld()->SpawnActor(MH_VRCharacterClass, &SpawnLocation));
	}
	
	MyController->UnPossess();
	MyController->Possess(Character);
	NowPawn->Destroy();

}