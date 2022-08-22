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
protected:
	UFUNCTION(BlueprintCallable)
		void JoinLevel();

};