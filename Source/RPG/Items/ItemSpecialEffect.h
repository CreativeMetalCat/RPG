// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"
#include "ItemSpecialEffect.generated.h"

/*Used by items that create special effects on use. For example: Magic wand that shoot lightning,or grenade that explodes*/
UCLASS(Blueprintable,notplaceable, meta=(ChildCanTick, KismetHideOverrides = "ReceiveAnyDamage,ReceivePointDamage,ReceiveRadialDamage,ReceiveActorBeginOverlap,ReceiveActorEndOverlap,ReceiveHit,ReceiveDestroyed,ReceiveActorBeginCursorOver,ReceiveActorEndCursorOver,ReceiveActorOnClicked,ReceiveActorOnReleased,ReceiveActorOnInputTouchBegin,ReceiveActorOnInputTouchEnd,ReceiveActorOnInputTouchEnter,ReceiveActorOnInputTouchLeave"), HideCategories=(Collision,Rendering,"Utilities|Transformation"))
class RPG_API AItemSpecialEffect : public AActor
{
	GENERATED_BODY()
	public:
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void ApplyEffect(AActor*CreatorOfEffect,FVector Origin);
};
