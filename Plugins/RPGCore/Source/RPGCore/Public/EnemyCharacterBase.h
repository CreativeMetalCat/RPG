// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AI/AIMovementType.h"
#include "RPGCore/Public/RPGCharacterBase.h"
#include "EnemyCharacterBase.generated.h"

/**
 * 
 */
UCLASS()
class RPGCORE_API AEnemyCharacterBase : public ARPGCharacterBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "AI|Movement|Random")
	float MaxDistanceOfRandomMovement = 300.f;

	/*Because random movement isn't about getting to some point, but rather creating imitation of ai being busy, game doesn't care about whether ai reached point or not*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "AI|Movement|Random")
	float RandomMovementTime = 3.f;

	/*
	*If false - points will be generated from location where ai currently stands
	* If true - points will be generated from location where ai was spawned
	* That's one very long name :)
	*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "AI|Movement|Random")
	bool bGenerateRandomPointsFromDefaultLocation = true;

	
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "AI|Movement")
	EAIMovementType MovementType = EAIMovementType::EAIMT_Random;
};
