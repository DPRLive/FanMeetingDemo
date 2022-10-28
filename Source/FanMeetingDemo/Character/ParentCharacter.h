// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "ParentCharacter.generated.h"

UCLASS()
class FANMEETINGDEMO_API AParentCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AParentCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Voice
	UFUNCTION(BlueprintCallable)
		bool GetIsVoiceChatOn() { return IsVoiceChatOn; }

	UPROPERTY(ReplicatedUsing = OnRep_ForcedMuteOnOff, BlueprintReadWrite)
		bool ForcedMute = false;

	UFUNCTION()
		void VoiceChatOnOff();

	UFUNCTION()
		void OnRep_ForcedMuteOnOff();

	void SetChangeMesh(USkeletalMesh* NewSkeletalMesh);

	UWidgetComponent* GetNamePlate() { return NamePlate; }
protected:
	virtual void BeginPlay() override;

	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UCameraComponent* Camera;

	//NamePlate
	UPROPERTY(EditAnywhere)
		UWidgetComponent* NamePlate;

	void NamePlateUpdate();

	UPROPERTY(Replicated, BlueprintReadWrite)
		FString PlayerName;

	UPROPERTY(ReplicatedUsing = OnRep_PlayerNameRef, BlueprintReadWrite)
		FString PlayerNameRef;

	UFUNCTION()
		void OnRep_PlayerNameRef();

	//Voice
	bool IsVoiceChatOn = false;

	//Menu
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UMenuWidget> MenuWidget;

	UPROPERTY() // UPROPERTY() Ãß°¡
		UMenuWidget* InGameUI = nullptr;

	//Movement
	UFUNCTION()
		virtual void MoveForward(float Scale) {};

	UFUNCTION()
		virtual void MoveRight(float Scale) {};

	UPROPERTY(ReplicatedUsing = OnRep_ChangeMesh)
		USkeletalMesh* ChangeMesh;

	UFUNCTION()
		void OnRep_ChangeMesh();
};
