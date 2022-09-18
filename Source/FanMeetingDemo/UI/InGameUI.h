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

	UFUNCTION(BlueprintCallable)
		bool GetShowPlayerName() { return ShowPlayerName; }
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

	// VR Setting
	UFUNCTION(BlueprintCallable)
		void BlinkerOnOff();

	UFUNCTION(BlueprintCallable)
		void ResetVR();

	// Game Setting
	UFUNCTION(BlueprintCallable)
		void TogglePlayerName();

private:
	bool ShowPlayerName = true;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AParentCharacter> CharacterClass;
};
