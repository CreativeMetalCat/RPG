// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIInterface.h"
#include "AIMovementType.h"
#include "PatrolPointBase.h"



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
	AEnemyAIBase();
	
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

	/*This actor will be used as "representation" of current Random Movement goal location
	 * It exists to make random movement work with Behaviour Tree
	 */
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "Movement|Random")
	AActor* RandomMovementGoalLocationActor = nullptr;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "Movement|Random")
	bool bRandomGoalActorIsSet = false;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "Movement|Patrolling")
	TArray<APatrolPointBase*> PatrolPoints;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "Movement|Patrolling")
	int CurrentPatrolPointId = -1;

	/*
	 * If true - Patrol points will be selected randomly
	 * If false - Patrol points will be selected in order that they were added to the list
	 */
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "Movement|Patrolling")
	bool bRandomPatrol = false;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "Movement|Patrolling|Blackboard")
	FName BlackboardPatrolPointTargetName = TEXT("PatrolPoint");

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "Movement|Patrolling|Blackboard")
	FName BlackboardWaitingName = TEXT("Waiting");

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "Movement|Target|Blackboard")
	FName BlackboardTargetName = TEXT("Target");
	
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = Movement)
	EAIMovementType MovementType = EAIMovementType::EAIMT_Random;

	UFUNCTION(BlueprintCallable,Category="Movement|Patrolling")
	void SetNewPatrolPoint();

	UFUNCTION(BlueprintCallable,Category="Movement|Patrolling")
	void OnReachedPatrolPoint();

	UFUNCTION(BlueprintCallable,Category="Movement|Random")
    void OnReachedGoalOfRandomMovement();

	UFUNCTION(BlueprintCallable,Category="Movement|Random")
    void SetNextGoalForRandomMovement();

	virtual void BeginPlay() override;

	
	virtual void SetNewTarget_Implementation(AActor* Target) override;

	virtual FVector GetCurrentRandomPointGoal_Implementation() override { return RandomMovementGoal; }
};
