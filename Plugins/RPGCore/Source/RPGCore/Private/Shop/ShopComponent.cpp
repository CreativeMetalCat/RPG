// Fill out your copyright notice in the Description page of Project Settings.


#include "Shop/ShopComponent.h"

/*Small macro that returns current seller */
#define GET_SELLER(boolCheck) (boolCheck?Seller:Buyer)


UShop::UShop()
{
	
}


TArray<FItemInfo> UShop::GetItemsToSell(bool isSeller)const
{
	if(GET_SELLER(isSeller))
	{
		TArray<FItemInfo> Result  = TArray<FItemInfo>();

		for(int i=0;i<GET_SELLER(isSeller)->Items.Num();i++)
		{
			if(GET_SELLER(isSeller)->Items[i].bForSale)
			{
				Result.Add(GET_SELLER(isSeller)->Items[i]);
			}
		}
		return Result;
	}
	else
	{
		return TArray<FItemInfo>();
	}
}

bool UShop::SellItem(bool isSellerSelling,FString itemName,int Amount)
{
	if(!isSellerSelling && !bCanBuyerSell){return false;}
	
	if(GET_SELLER(isSellerSelling) != nullptr)
	{
		bool hasItem;
		const FItemInfo info = GET_SELLER(isSellerSelling)->GetItemByName(itemName,hasItem);
		const int MoneyReward = info.Cost*Amount;

		GET_SELLER(isSellerSelling)->RemoveItem(itemName,Amount);
		/*GET_SELLER(!isSellerSelling) is a fast but not pretty way of getting opposite side of the deal*/
		GET_SELLER(!isSellerSelling)->AddMoney(MoneyReward);
		
		return true;
	}
	return false;
}

int UShop::GetAmountOfItem(FString itemName, bool isSeller)
{
	if(GET_SELLER(isSeller) != nullptr)
	{
		bool hasItem;
		return GET_SELLER(isSeller)->GetItemAmountByName(itemName,hasItem);	
	}
	return -1;
}

