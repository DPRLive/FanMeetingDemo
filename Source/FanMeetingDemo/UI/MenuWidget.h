// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Character/ParentCharacter.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class FANMEETINGDEMO_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;

	void Setup();

	void Teardown();

	bool IsSetup() { return bSetup; }

	void SetOwner(AParentCharacter* NewOwner) { Owner = NewOwner; }

	void SetOwnerPlatformType(int32 NewPlayformType) { PlatformType = NewPlayformType; }

	AParentCharacter* GetOwner() { return Owner; }

protected:
	UPROPERTY(BlueprintReadOnly)
		int32 PlatformType = -1;

	UPROPERTY(BlueprintReadOnly)
		AParentCharacter* Owner;

private:
	bool bSetup = false;
};
