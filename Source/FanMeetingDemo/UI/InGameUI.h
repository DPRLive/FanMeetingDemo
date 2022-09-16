// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "InGameUI.generated.h"

/**
 * 
 */
UCLASS()
class FANMEETINGDEMO_API UInGameUI : public UMenuWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;

protected:
	UFUNCTION(BlueprintCallable)
		void GoMainMenu();

	UFUNCTION(BlueprintCallable)
		void Close();

	// Voice Setting
	UFUNCTION(BlueprintCallable)
		void MicOnOff();

	UFUNCTION(BlueprintCallable)
		void SetMicVolume(float Value);
};
