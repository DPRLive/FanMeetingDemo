// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
// custom headers
#include "../Character/ParentCharacter.h"

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
	void SetCharacterRef(AParentCharacter* Character) { CharacterRef = Character; }
protected:
	UPROPERTY(BlueprintReadOnly)
		AParentCharacter* CharacterRef;
private:

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
		UTextBlock* PlayerName;
};
