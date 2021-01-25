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
	if(Crafter && Recipe.Ingredients.Num() > 0 && Recipe.ResultItem.Count > 0)
	{
		bool has = false;
		int amount = 0;
		//check if Character has items
		for (int i = 0; i < Recipe.Ingredients.Num(); i++)
		{
			amount = Crafter->GetItemAmountByName(Recipe.Ingredients[i].Info.RowName.ToString(), has);
			if (amount < Recipe.Ingredients[i].Count || !has)
			{
				//if not we say so
				return false;
			}
		}

		for (int i = 0; i < Recipe.Ingredients.Num(); i++)
		{
			Crafter->RemoveItem(Recipe.Ingredients[i].Info.RowName.ToString(),Recipe.Ingredients[i].Count);
		}
		
		if(Recipe.ResultItem.Info.DataTable)
		{
			if(FItemInfo* result = Recipe.ResultItem.Info.GetRow<FItemInfo>(""))
			{
				result->CurrentAmount = Recipe.ResultItem.Count;
				Crafter->AddItem(*result);
				return true;
			}			
		}
	}
	
	return false;
}

