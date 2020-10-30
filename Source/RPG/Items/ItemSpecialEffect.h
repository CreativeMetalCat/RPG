// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"
#include "ItemSpecialEffect.generated.h"

/*Used by items that create special effects on use. For example: Magic wand that shoot lightning,or grenade that explodes*/
UCLASS(Blueprintable)
class RPG_API UItemSpecialEffect : public UObject
{
	GENERATED_BODY()
	public:
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void ApplyEffect(AActor*CreatorOfEffect,FVector Origin);
};
