// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "RPGCharacterBase.h"
#include "UObject/Interface.h"
#include "AIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPGCORE_API IAIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=RandomMovement)
	FVector GetCurrentRandomPointGoal();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category="AI|Target")
	AActor* GetCurrentTarget();
	
	/*Passing nullptr MUST clear the value*/
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category="AI|Target")
	void SetNewTarget(AActor*Target);
	
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category="AI|Target")
	TArray<FName> GetEnemyTags();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category="AI|Sense")
	TArray<AActor*> GetCurrentlySensedActors();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category="Saving")
	FVector GetLastSeenLocation();
};
