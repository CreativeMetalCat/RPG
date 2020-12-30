// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile2dBase.h"

// Sets default values
AProjectile2dBase::AProjectile2dBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	
	ProjectileDisplayComponent  = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("ProjectileDisplay"));
	
}

// Called when the game starts or when spawned
void AProjectile2dBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile2dBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

