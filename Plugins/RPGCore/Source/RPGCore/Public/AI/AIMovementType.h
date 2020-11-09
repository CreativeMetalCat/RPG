// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "AIMovementType.generated.h"

/**
 * 
 */
UENUM()
enum class EAIMovementType: uint8
{
	EAIMT_Stationary UMETA(DisplayName = "Stationary"),
    EAIMT_Random UMETA(DisplayName = "Random"),
    EAIMT_Points UMETA(DisplayName = "PatrolPoints"),
};
