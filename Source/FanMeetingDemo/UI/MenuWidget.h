// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class FANMEETINGDEMO_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Setup();

	void Teardown();

	bool IsSetup() { return bSetup; }

	void SetOwner(ACharacter* NewOwner) { Owner = NewOwner; }

	void SetOwnerPlatformType(int32 NewPlayformType) { PlatformType = NewPlayformType; }

	ACharacter* GetOwner() { return Owner; }

protected:
	UPROPERTY(BlueprintReadOnly)
		int32 PlatformType = -1;

	UPROPERTY(BlueprintReadOnly)
		ACharacter* Owner;

private:
	bool bSetup = false;
};
