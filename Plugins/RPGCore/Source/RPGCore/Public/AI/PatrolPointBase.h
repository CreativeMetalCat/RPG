// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPointBase.generated.h"

UCLASS()
class RPGCORE_API APatrolPointBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrolPointBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	/*
	 * How long will ai wait on this point before going to the next one
	 */
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= AI)
	float WaitTime = 3.f;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
