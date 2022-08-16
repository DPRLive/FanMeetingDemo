// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
// custom headers
#include "../Character/PCCharacter.h"
#include "../Character/VRCharacter.h"

// unreal headers
#include "Components/TextBlock.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NamePlate.generated.h"

/**
 * 
 */
UCLASS()
class FANMEETINGDEMO_API UNamePlate : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetName(const FString& NewName) { PlayerName->SetText(FText::FromString(NewName)); }
	void SetPCCharacterRef(APCCharacter* Character);
	void SetVRCharacterRef(AVRCharacter* Character);
protected:
	UPROPERTY(BlueprintReadOnly)
		APCCharacter* PCCharacterRef;

	UPROPERTY(BlueprintReadOnly)
		AVRCharacter* VRCharacterRef;

	UPROPERTY(BlueprintReadOnly)
		APlayerController* PlayerControllerRef;
private:

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
		UTextBlock* PlayerName;
};
