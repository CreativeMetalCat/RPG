// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"
#include "Engine/UserDefinedStruct.h"
#include "Quest.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FQuest :public
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

 /*What player has to do*/
 UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Display)
 FText Description;

 /*All quests have progress(kill n skeltons, find x coins etc.), quest is considered completed ONLY if NeededProgress = CurrentProgress
  * If quest doesn't require doing more than one thing(for example: player only needs to kill one monster) then leave this value as 1 and number won't be displayed 
  * Progress can be made EVEN before quest was given to player ONLY if bAllowProgressBeforeGiven is true
  */
 UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Progress)
 int NeededProgress = 1;

 /*
 * This value should only be updated via functions in player itself
 */
 UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Progress)
 int CurrentProgress = 0;

 /*
  * Progress can be made EVEN before quest was given to player ONLY if bAllowProgressBeforeGiven is true
  * HOWEVER, quest can not be finished if bShow is true
  */
 UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Progress)
 bool bAllowProgressBeforeGiven = true;

 /*When game starts player already has all quests, but game can only mark it as completed if was given to player.
  *if bShow is true than player was given this quest, if false - not*/
 UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Dev)
 bool bShow = false;

 /*DO NOT TOUCH THIS. THIS VALUE IS FOR CODE*/
 UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Dev)
 bool bCompleted = false;
};
