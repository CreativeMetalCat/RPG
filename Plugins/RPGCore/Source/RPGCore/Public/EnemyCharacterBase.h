// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "AI/AIInterface.h"
#include "AI/AIMovementType.h"
#include "Components/SphereComponent.h"
#include "RPGCore/Public/RPGCharacterBase.h"
#include "RPGCore/Public/AI/PatrolPointBase.h"
#include "EnemyCharacterBase.generated.h"

/**
 * This is base class for every enemy existing in the world
 */
UCLASS()
class RPGCORE_API AEnemyCharacterBase : public ARPGCharacterBase, public IAIInterface
{
	GENERATED_BODY()
public:

	AEnemyCharacterBase();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI|Sense")
	USphereComponent*SenseSphereComponent;
	
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
	
	/*Points used by ai for patrolling*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "AI|Movement|Patrolling")
	TArray<APatrolPointBase*> PatrolPoints;
	
	/*
	* If true - Patrol points will be selected randomly
	* If false - Patrol points will be selected in order that they were added to the list
	*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "AI|Movement|Patrolling")
	bool bRandomPatrol = false;

	
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "AI|Movement")
	EAIMovementType MovementType = EAIMovementType::EAIMT_Random;

	UFUNCTION(BlueprintCallable,Category = "AI|Update")
	void UpdateAI();

	virtual void BeginPlay() override;

	virtual ARPGCharacterBase* GetCurrentTarget_Implementation() override{return CurrentTarget;}
	
	public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "AI|Target")
	ARPGCharacterBase* CurrentTarget = nullptr;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category =  "AI|Target|Classes",meta=(ExposeOnSpawn = true))
	TArray<FName> EnemyTags;

	virtual TArray<FName> GetEnemyTags_Implementation() override;

	virtual void GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const override;

	UFUNCTION()
	void OnAttackCollisionOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
