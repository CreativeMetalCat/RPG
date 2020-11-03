// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"
#include "SpecialEffect.generated.h"

/*Used by items that create special effects on use. For example: Magic wand that shoot lightning,or grenade that explodes*/
UCLASS(Blueprintable,notplaceable, meta=(ChildCanTick, KismetHideOverrides = "ReceiveAnyDamage,ReceivePointDamage,ReceiveRadialDamage,ReceiveActorBeginOverlap,ReceiveActorEndOverlap,ReceiveHit,ReceiveDestroyed,ReceiveActorBeginCursorOver,ReceiveActorEndCursorOver,ReceiveActorOnClicked,ReceiveActorOnReleased,ReceiveActorOnInputTouchBegin,ReceiveActorOnInputTouchEnd,ReceiveActorOnInputTouchEnter,ReceiveActorOnInputTouchLeave"), HideCategories=(Collision,Rendering,"Utilities|Transformation"))
class RPGCORE_API ASpecialEffect : public AActor
{
	GENERATED_BODY()
	public:

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= Info)
	AActor*Creator = nullptr;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= Info)
	FVector EffectOrigin = FVector::ZeroVector;
	
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
    void ApplyEffect(AActor*CreatorOfEffect,FVector Origin);

	/* Called when effect's life span ends
	* Called only if bDoesEffectLasts = true
	*/
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
    void OnEffectEnds();

	/*If false effect will be fired and that's all(like an explosion) if true effect will last for some time(like poison)*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= EffectTime)
	bool bDoesEffectLasts = false;

	/*How long will this effect last for
	* bDoesEffectLasts must be true for this to take effect
	*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= EffectTime)
	float EffectLifeSpan = 1.f;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= Setup)
	bool bLasts = false;
};
