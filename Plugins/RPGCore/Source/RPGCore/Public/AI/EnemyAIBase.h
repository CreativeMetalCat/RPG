// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIInterface.h"
#include "AIMovementType.h"


#include "EnemyAIBase.generated.h"

/**
 * 
 */
UCLASS()
class RPGCORE_API AEnemyAIBase : public AAIController, public IAIInterface
{
	GENERATED_BODY()
protected:
	FTimerHandle RandomMovementTimerHandle;
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "Movement|Random")
	float MaxDistanceOfRandomMovement = 300.f;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "Movement|Random")
	FVector RandomMovementGoal;

	/*Because random movement isn't about getting to some point, but rather creating imitation of ai being busy, game doesn't care about whether ai reached point or not*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "Movement|Random")
	float RandomMovementTime = 3.f;

	/*
	 *If false - points will be generated from location where ai currently stands
	 * If true - points will be generated from location where ai was spawned
	 * That's one very long name :)
	 */
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "Movement|Random")
	bool bGenerateRandomPointsFromDefaultLocation = true;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "Movement|Random")
	FVector SpawnLocation;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = Movement)
	EAIMovementType MovementType = EAIMovementType::EAIMT_Random;

	UFUNCTION(BlueprintCallable,Category=RandomMovement)
    void OnReachedGoalOfRandomMovement();

	UFUNCTION(BlueprintCallable,Category=RandomMovement)
    void SetNextGoalForRandomMovement();

	virtual void BeginPlay() override;

	virtual FVector GetCurrentRandomPointGoal_Implementation() override { return RandomMovementGoal; }
};
