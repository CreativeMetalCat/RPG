// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"
#include "SpecialEffect.generated.h"

/*This enum exists only as code enhancement and only needed if effect is applied to weapon
 * This will only be checked by abilities that apply something to weapons(but not everyone of them)
 */
UENUM(BlueprintType)
enum class EEffectType:uint8
{
	EET_None UMETA(DisplayName = "None"),
	EET_Fire UMETA(DisplayName = "Fire"),
	EET_Electricity UMETA(DisplayName = "Electricity")
};

/*Used by items that create special effects on use. For example: Magic wand that shoot lightning,or grenade that explodes*/
UCLASS(Blueprintable,notplaceable, meta=(ChildCanTick, KismetHideOverrides = "ReceiveAnyDamage,ReceivePointDamage,ReceiveRadialDamage,ReceiveActorBeginOverlap,ReceiveActorEndOverlap,ReceiveHit,ReceiveDestroyed,ReceiveActorBeginCursorOver,ReceiveActorEndCursorOver,ReceiveActorOnClicked,ReceiveActorOnReleased,ReceiveActorOnInputTouchBegin,ReceiveActorOnInputTouchEnd,ReceiveActorOnInputTouchEnter,ReceiveActorOnInputTouchLeave"), HideCategories=(Collision,Rendering,"Utilities|Transformation"))
class RPGCORE_API ASpecialEffect : public AActor
{
	GENERATED_BODY()
	protected:
	FTimerHandle EffectLoopTimer;
	public:

	/*Name that players will see*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Display)
	FText DisplayName;

	/*What player has to do*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Display)
	FText Description;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= Timer)
	FTimerHandle EffectTimer;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= Info)
	AActor*Creator = nullptr;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= Info)
	AActor*Target = nullptr;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= Info)
	FVector EffectOrigin = FVector::ZeroVector;

	/*Id of ability that uses this effect
	 * DO NOT CHANGE THIS VALUE. IT IS CHANGED BY PLAYER CODE
	 */	
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= "Info|Ability")
	int AbilityId = -1;

	/*This will only be checked by abilities that apply something to weapons(but not everyone of them)*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= "Info|Type")
	EEffectType Type = EEffectType::EET_None;

	/* How much damage this ability should deal
	 * IMPORTANT: It's important to know that this damage is not obligatory to use(for example abilities that heal don't even deal damage
	 * Main reason to use it when you need damage to be affected by character's attack points by 
	 */
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= Damage)
	float Damage = 0.f;

	/*This effect will be applied along side weapons already existing weapon effect
	 * For example - fire(Some ability Enhances weapon to be on fire and each time weapon damages it's effect applies as well
	 */
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="WeaponEnhancement")
	TSubclassOf<ASpecialEffect> WeaponEnhancementEffect;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="WeaponEnhancement")
	TArray<EEffectType> IncompatibleTypes;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="WeaponEnhancement")
	bool bHasWeaponEnhancement = false;

	/*
	 * TargetActor - If your effect is supposed to be applied directly to some actor
	 */
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
    void ApplyEffect(AActor*CreatorOfEffect,FVector Origin,UWorld *WorldContext, AActor*TargetActor = nullptr);

	/* Called when effect's life span ends
	* Called only if bDoesEffectLasts = true
	*/
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
    void OnEffectEnds();

	/* if bDoesEffectLasts is true this function will be called each EffectLoopTime
	 * 
	 */
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void OnLoop();

	/*If false effect will be fired and that's all(like an explosion) if true effect will last for some time(like poison)*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= EffectTime)
	bool bDoesEffectLasts = false;

	/*How long will this effect last for
	* bDoesEffectLasts must be true for this to take effect
	*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= EffectTime)
	float EffectLifeSpan = 1.f;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= EffectTime)
	float EffectLoopTime = 0.1f;

	/*If true effect will be applied only to TargetActor(doesn't actually affect how effect works. Needed for weapons system)
	 * 
	 */
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= Info)
	bool bLocal = false;

	/*This function is called when button corresponing to slot for this ability is releasedS
	 * Requires to be binded via IInteraction::BindEventOnAbilityButtonReleased(ASpecialEffect*Effect) first
	 */
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=Player)
	void OnAbilityButtonReleased();

};
