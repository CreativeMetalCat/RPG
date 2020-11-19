// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"
#include "Engine/UserDefinedStruct.h"
#include "RPGCore/Public/Item/SpecialEffect.h"

#include "AbilityInfo.generated.h"

/**
* 
*/
USTRUCT(BlueprintType)
struct FAbilityInfo :public
FTableRowBase
{
	GENERATED_BODY()
public:
	/*MUST BE UNIQUE.
	* Only visible by developers inside the editor
	*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Dev)
	FString DevName;

	/*Name that players will see*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Display)
	FText DisplayName;

	/*What player has to do*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Display)
	FText Description;
    
	/*Type of ability that this is*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= Setup)
	TSubclassOf<ASpecialEffect> AbilityClass;

   

	/*Set to 0 if this ability doesn't cost anything*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= Cost)
	int MannaUsage = 0;

	/*Set to 0 if this ability doesn't cost anything*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= Cost)
	int HealthCost = 0;

	/*How much skillpoints are needed to open this ability*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= SkillPointCost)
	int SkillPointCost = 1;

	/*
	* This ability can not be gotten before all this are gotten
	* For abilities that are upgrades
	*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Requirements)
	TArray<FString> NeededAbilities;


	/*If true "ApplyEffect" of Special Effect will NOT be called
	 * If false "ApplyEffect" of Special Effect WILL be called
	 */
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="WeaponEnhancement")
	bool IsOnlyWeaponEnhancement = false;

	/*If true than target of ability will be also the one who spawned it
	 * Avoid setting it to true if ability deals damage without any checks
	 */
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Effect")
	bool bApplyToSpawner = false;

	
	UPROPERTY(BlueprintReadOnly,BlueprintReadOnly,Category=Cooldown)
	FTimerHandle CooldownTimerHandle;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Cooldown)
	bool bIsCoolingdown = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Cooldown)
	float CooldownTime = 1.f;
};