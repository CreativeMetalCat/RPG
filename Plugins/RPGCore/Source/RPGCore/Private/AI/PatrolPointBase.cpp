// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGCore/Public/AI/PatrolPointBase.h"

// Sets default values
APatrolPointBase::APatrolPointBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APatrolPointBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APatrolPointBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

