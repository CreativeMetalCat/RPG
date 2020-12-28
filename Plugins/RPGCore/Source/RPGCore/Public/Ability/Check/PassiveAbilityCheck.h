// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PassiveAbilityCheck.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class RPGCORE_API UPassiveAbilityCheck : public UObject
{
	GENERATED_BODY()
	public:
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	bool DoCheck(class ARPGCharacterBase* character);
};
