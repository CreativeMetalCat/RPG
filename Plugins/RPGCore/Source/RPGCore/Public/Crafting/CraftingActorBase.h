// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "Recipe.h"
#include "RPGCharacterBase.h"
#include "GameFramework/Actor.h"
#include "CraftingActorBase.generated.h"

UCLASS()
class RPGCORE_API ACraftingActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACraftingActorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	bool Craft(ARPGCharacterBase*Crafter,FRecipe Recipe);
};
