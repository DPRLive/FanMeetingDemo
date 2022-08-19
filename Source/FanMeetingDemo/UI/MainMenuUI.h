// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/EditableTextBox.h"
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
	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
protected:
	//UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget))
	//	UButton* PCBtn;

	//UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget))
	//	UButton* VRBtn;

	//UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (BindWidget))
	//	UEditableTextBox* IDBox;

	//UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (BindWidget))
	//	UEditableTextBox* PWBox;

	UFUNCTION(BlueprintCallable)
		void JoinLevel();
private:
	bool IsShowKeyboard = false;

	class FSlateApplication* FSA;
//	UFUNCTION()
//		void PCBtnClicked();
//
//	UFUNCTION()
//		void VRBtnClicked();
};
