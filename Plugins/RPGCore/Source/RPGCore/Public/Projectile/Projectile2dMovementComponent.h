// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "Projectile2dMovementComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=Movement,Blueprintable, meta=(BlueprintSpawnableComponent))
class RPGCORE_API UProjectile2dMovementComponent : public UActorComponent
{
	GENERATED_BODY()
	protected:
	
	FVector CurrentVelocity;
	
	public:

	UProjectile2dMovementComponent();
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Velocity)
	float MaxSpeed = 100.f;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Velocity)
	FVector InitialVelocity;

	/*Set to -1 to use default tick time*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Update)
	float UpdateInterval = -1.f;

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly,Category=Component)
	USceneComponent *ComponentToMove = nullptr;

	UFUNCTION(BlueprintCallable)
	virtual void Init();

	UFUNCTION(BlueprintCallable)
	void SetNewInitialVelocity(FVector newVel);

	UFUNCTION(BlueprintCallable)
    void SetMaxSpeed(float speed);
	
	virtual void InitializeComponent()override;

	virtual FVector GetCurrentVelocity() const
	{
		return CurrentVelocity;
	}
	
	virtual float GetMaxSpeed()const 
	{
		return MaxSpeed;
	}

	virtual void BeginPlay()override;
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
};
