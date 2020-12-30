// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PaperFlipbookComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Projectile2dBase.generated.h"

UCLASS()
class RPGCORE_API AProjectile2dBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile2dBase();

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	USceneComponent *SceneComponent;

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	UPaperFlipbookComponent *ProjectileDisplayComponent;
	
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	USphereComponent *CollisionComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
