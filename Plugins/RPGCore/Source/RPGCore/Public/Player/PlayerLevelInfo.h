// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"
#include "Engine/UserDefinedStruct.h"
#include "PlayerLevelInfo.generated.h"

/**
 * Struct that holds info about levels that player reaches when levels up
 */
USTRUCT(BlueprintType)
struct FPlayerLevelInfo : public FTableRowBase
{
 GENERATED_BODY()
public:
 /*Currently unused*/
 UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=Exp)
 int NeededExp = 100;

 /*How much skill points will player get when they will reach this level*/
 UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=SkillPoints)
 int SkillPoints = 0;

 /*What abilities can player now get. May go unused based on design of skill tree*/
 UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=SkillPoints)
 TArray<FDataTableRowHandle>UnlockedAbilities;
};
