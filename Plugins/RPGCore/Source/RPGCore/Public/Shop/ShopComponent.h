// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "RPGCharacterBase.h"
#include "Components/ActorComponent.h"
#include "ShopComponent.generated.h"


UCLASS(Blueprintable)
class RPGCORE_API UShop : public UObject
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UShop();

public:

	/*The one who came to buy stuff*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ARPGCharacterBase* Buyer = nullptr;
	
	/*The one who came to sell stuff*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ARPGCharacterBase* Seller = nullptr;

	/*If false buyer can not sell own items for gold*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bCanBuyerSell = true;

	/*Returns list of items that seller can sell
	 * WARNING: these are not actual items but copies of their info
	* if Seller is true -> get's what Seller has
	* if false -> what Buyer has 
	 */
	UFUNCTION(BlueprintCallable)
	TArray<FItemInfo> GetItemsToSell(bool isSeller)const;
	
	/*Safely remove item and give money
	 * if isSellerSelling is true -> seller looses item and get's money
	 * if false -> seller gets item and losses money
	 */
	UFUNCTION(BlueprintCallable)
	bool SellItem(bool isSellerSelling,FString itemName,int Amount);

	/*Returns amount of item actor has
	 * if Seller is true -> get's how many Seller has
	 * if false -> how many Buyer has 
	 */
	UFUNCTION(BlueprintPure)
	int GetAmountOfItem(FString itemName,bool isSeller);
};
