// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"
#include "Engine/UserDefinedStruct.h"
#include "QuestInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FQuestInfo :public
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

 /*
  * If set to 0 this reward will NOT be displayed
  */
 UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Reward)
 int ExperienceReward = 0;

 /*
 * If set to 0 this reward will NOT be displayed
 */
 UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Reward)
 int MoneyReward = 0;

 /*
  * This quest can not be started until all this are completed
  */
 UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Requirements)
 TArray<FString> NeededQuests;

 /*When this quest is completed all quests that are stated there will be progressed by stated number
  * This can be used for quests that are build of subquests
  * Example: Quest: Fix windmill. Subquests:Find gear, find string, fix rotating mechanism. When any of those are completed Main quest gets progressed
  */
 UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Quests)
 TMap<FString,int> QuestsToAffect;

 UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Quests)
 bool bCanAffectOtherQuests = true;

 /*Used ONLY for ui, has no actual effect on gameplay*/
 UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Dev|UI")
 bool bSubQuest = false;

 /*When game starts player already has all quests, but game can only mark it as completed if was given to player.
  *if bShow is true than player was given this quest, if false - not*/
 UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Dev)
 bool bShow = false;

 /*
  * Used if reward for quest isn't given right away(player has to talk to someone to get reward)
  * Changed ONLY in code
  */
 UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Dev)
 bool bRewarded = false;

 /*DO NOT TOUCH THIS. THIS VALUE IS FOR CODE*/
 UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Dev)
 bool bCompleted = false;
};