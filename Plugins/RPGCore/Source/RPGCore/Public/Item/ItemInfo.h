// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "SpecialEffect.h"
#include "Engine/DataTable.h"
#include "Engine/UserDefinedStruct.h"
#include "ItemInfo.generated.h"

UENUM(Blueprintable)
enum class EItemType :uint8
{
    EIT_Weapon UMETA(DisplayName = "Weapon"),
    EIT_ArmorTop UMETA(DisplayName = "ArmorTop"),
    EIT_ArmorMiddle UMETA(DisplayName = "ArmorMiddle"),
    EIT_ArmorBottom UMETA(DisplayName = "ArmorBottom"),
    EIT_Shield UMETA(DisplayName = "Shield"),
    EIT_Consumable UMETA(DisplayName = "Consumable"),
    EIT_KeyItems UMETA(DisplayName = "KeyItems"),
    EIT_Materials UMETA(DisplayName = "Materials"),
    //Used for loot that drops from enemies
    EIT_Money UMETA(DisplayName = "Money"),
    EIT_Other UMETA(DisplayName = "Other",Description = "For items that can not be worn or used")
};

/**
 * Struct that has all info about the item. Please note that instances of the item in the world must not be changed
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

   /* Image used to represent this item in ui(icon)
   * Only `Image` property of this property is important
   */
   UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Image")
   FSlateBrush UIDisplayImage;

   /* Image used to represent this item in world
   * Used only if bUseSprite is true
   */
   UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Image")
   UPaperSprite* WorldDisplaySprite;

   /* Image used to represent this item in world
   * Used only if bUseSprite is false
   */
   UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Image")
   UPaperFlipbook* WorldDisplayPaperFlipbook;

   UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Image")
   bool bUseSprite = true;
 
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

    /*How much character's speed will decrease when shield is drawn*/
    UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Shield")
    float MovementSpeedDecreaseMultiplier = 0.5;

    /*Used by items that create special effects on use. For example: Magic wand that shoot lightning,or grenade that explodes*/
    UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Stats)
    TSubclassOf<ASpecialEffect> SpecialEffect; 

    /**
     * What type of item this is
     * Use `other` for items that can not be used worn or anything else
     */
    UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Type)
    EItemType Type = EItemType::EIT_Other;

    /*For Weapons. What kind of damage this weapon deals*/
    UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Weapons_Damage)
    TSubclassOf<UDamageType> DamageType = UDamageType::StaticClass();

    /*If player can cary more than of item of this type in one slot*/
    UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Count)
    bool bCanBeStacked = false;

    /*How many of this items can player cary in one slot*/
    UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Count)
    int32 MaxStackAmount = 2;

    /*How many items are in this slot*/
    UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Count)
    int32 CurrentAmount = 0;
};