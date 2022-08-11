// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Button.h"
#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MainMenuUI.generated.h"

/**
 * 
 */
UCLASS()
class FANMEETINGDEMO_API UMainMenuUI : public UMenuWidget
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget))
		UButton* PCBtn;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget))
		UButton* VRBtn;

private:
	UFUNCTION()
		void PCBtnClicked();

	UFUNCTION()
		void VRBtnClicked();
};
