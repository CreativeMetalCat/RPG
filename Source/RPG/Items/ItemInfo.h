// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "ItemSpecialEffect.h"
#include "Engine/DataTable.h"
#include "Engine/UserDefinedStruct.h"
#include "ItemInfo.generated.h"

UENUM(Blueprintable)
enum class EItemType :uint8
{
    EIT_Weapon UMETA(DisplayName = "Weapon"),
    EIT_Armor UMETA(DisplayName = "Armor"),
    EIT_Other UMETA(DisplayName = "Other",Description = "For items that can not be worn or used")
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct FItemInfo :public
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

    /**
     * All items share values like attack, defence etc. But game will only care about those values(abd display them) if they fit the type
     */
    UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Stats)
    int Attack = 0;

    /**
    * All items share values like attack, defence etc. But game will only care about those values(abd display them) if they fit the type
    */
    UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Stats)
    int Defence = 0;

    UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Stats)
    int Cost = 0;

    /*Used by items that create special effects on use. For example: Magic wand that shoot lightning,or grenade that explodes*/
    UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Stats)
    TSubclassOf<UItemSpecialEffect> SpecialEffect; 

    /**
     * What type of item this is
     * Use `other` for items that can not be used worn or anything else
     */
    UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Type)
    EItemType Type = EItemType::EIT_Other;
};
