// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "AI/AIInterface.h"
#include "AI/AIMovementType.h"
#include "AI/EnemyAIBase.h"
#include "Components/SphereComponent.h"
#include "Item/PickupableItemBase.h"
#include "RPGCore/Public/RPGCharacterBase.h"
#include "RPGCore/Public/AI/PatrolPointBase.h"
#include "EnemyCharacterBase.generated.h"

/*Simple struct to replace the TMap for loot because FDataTableRowHandle can not be used in Tmap*/
USTRUCT(BlueprintType)
struct FLootTypeInfo
{
	GENERATED_BODY()
	public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FDataTableRowHandle Info;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int Amount;
};
/**
 * This is base class for every enemy existing in the world
 */
UCLASS()
class RPGCORE_API AEnemyCharacterBase : public ARPGCharacterBase, public IAIInterface
{
	GENERATED_BODY()
protected:
	FTimerHandle AttackCollisionCheckUpdateTimerHandle;
public:

	AEnemyCharacterBase();

	/*The controller to use if default controller is null on spawn */
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "AI")
	TSubclassOf<AEnemyAIBase> FallbackControllerClass = AEnemyAIBase::StaticClass();
	
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

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Update")
	float AttackCheckUpdateRate = 1.f;

	virtual void BeginPlay() override;
	
	public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "AI")
	bool bIsControllerCompatible = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "AI|Target")
	AActor* CurrentTarget = nullptr;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category =  "AI|Target|Classes",meta=(ExposeOnSpawn = true))
	TArray<FName> EnemyTags;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "AI|Attack")
	bool bAnythingOverlappingUpperCollision = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "AI|Attack")
	bool bAnythingOverlappingLowerCollision = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "AI|Attack")
	bool bAnythingOverlappingRightCollision = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "AI|Attack")
	bool bAnythingOverlappingLeftCollision = false;

	/*Items to drop where key is what to drop and value is how much*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Loot")
	TArray<FLootTypeInfo> ItemsToDrop;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Loot")
	TSubclassOf<APickupableItemBase> LootDropItemClass = APickupableItemBase::StaticClass();
	
	UFUNCTION(BlueprintCallable,Category = "AI|Attack")
	void CheckAttackCollision();
	

	virtual TArray<FName> GetEnemyTags_Implementation() override;

	virtual void GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const override;

	virtual void SetNewTarget_Implementation(AActor*Target)override;

	virtual void Die_Implementation() override;

	UFUNCTION()
	void OnAttackCollisionOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnAttackCollisionEndOverlap(UPrimitiveComponent* OverlappedComp,AActor* OtherActor, UPrimitiveComponent* OtherComp,int32 OtherBodyIndex);
};
