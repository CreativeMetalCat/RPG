// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "NavigationSystem.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "RPGCharacterBase.h"
#include "SpawnerBase.generated.h"

UCLASS()
class RPGCORE_API ASpawnerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	FTimerHandle SpawnUpdateTimerHandle;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	class USceneComponent*Scene;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=EditorOnly)
	class USphereComponent* RadiusDisplaySphere;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=EditorOnly)
	class UBillboardComponent* EditorBillboard;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UNavigationSystemV1* NavSystem;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Spawn|Class",meta=(ExposeOnSpawn = true))
	TSubclassOf<ARPGCharacterBase>ClassToSpawn;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Spawn|Area",meta=(ExposeOnSpawn = true))
	float SpawnRadius = 800.f;

	/*How ofter stuff will spawn*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Spawn|Rate",meta=(ExposeOnSpawn = true))
	float SpawnRate = 1.f;
	
	void UpdateAndSpawn();
};
