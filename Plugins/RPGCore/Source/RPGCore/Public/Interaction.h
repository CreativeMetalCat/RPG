// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Item/SpecialEffect.h"
#include "UObject/Interface.h"
#include "Interaction.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteraction : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPGCORE_API IInteraction
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category=Interaction)
    void Interact(AActor*Interactor);

	/*Use this instead of ApplyDamage(gives more control)
	 * Damage - How much damage to deal
	 * TSubclassOf<ASpecialEffect>SpecialEffect - What effect to apply to attacked actor(use for applying poison, fire etc,)
	 * Returns damage that was actually dealt
	 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category=Damage)
	int DealDamage(int Damage,AActor*DamageDealer =nullptr, TSubclassOf<ASpecialEffect>SpecialEffect = nullptr);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category=Effect)
	void NotifyAboutEffectStart(ASpecialEffect*Effect);

	/*Button is selected based on info in effect itself (AbilityId)*/
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category=Effect)
	void BindEventOnAbilityButtonReleased(ASpecialEffect*Effect);

	/*
	 * it same function as IInteraction::Interact, but separately from it to avoid casting to check if player or puzzle piece tried to interact
	 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category=Puzzles)
	void ToggleMechanism(AActor*activator);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category=Expirience)
	void AddExperience(int exp);
	
};
