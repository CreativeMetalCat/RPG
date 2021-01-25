// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"

#include "Recipe.generated.h"

USTRUCT(BlueprintType)
struct FIngredientInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FDataTableRowHandle Info;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 Count = 0;
};

/**
* 
*/
USTRUCT(BlueprintType)
struct FRecipe :public
FTableRowBase
{
	GENERATED_BODY()
public:
	/*MUST BE UNIQUE.
	* Only visible by developers inside the editor
	*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Dev)
	FString DevName;

	/*Name that players will see*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Display)
	FText DisplayName;

	/*What this thing is*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Display)
	FText Description;

	/* Image used to represent this item in ui(icon)
	* Only `Image` property of this property is important
	*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Image")
	FSlateBrush UIDisplayImage;

	//Must point to a table that uses FItemInfo as row base
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= "Food")
	TArray<FIngredientInfo> Ingredients;

	//Must point to a table that uses FItemInfo as row base
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= "Food")
	FDataTableRowHandle ResultItem;
};