// Fill out your copyright notice in the Description page of Project Settings.


#include "Crafting/CraftingActorBase.h"

// Sets default values
ACraftingActorBase::ACraftingActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ACraftingActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

bool ACraftingActorBase::Craft(ARPGCharacterBase* Crafter, FRecipe Recipe)
{
	if(Crafter && Recipe.Ingredients.Num() > 0)
	{
		bool has = false;
		int amount = 0;
		for(int i = 0;i < Recipe.Ingredients.Num();i++)
		{
			amount = Crafter->GetItemAmountByName(Recipe.Ingredients[i].Info.RowName.ToString(),has);
			if(!(amount >= Recipe.Ingredients[i].Count && has))
			{
				
			}
		}
	}
	return false;
}

