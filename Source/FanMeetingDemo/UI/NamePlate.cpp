// Fill out your copyright notice in the Description page of Project Settings.


#include "NamePlate.h"

void UNamePlate::SetPCCharacterRef(APCCharacter* Character)
{
	PCCharacterRef = Character;
}

void UNamePlate::SetVRCharacterRef(AVRCharacter* Character)
{
	VRCharacterRef = Character;
}
