// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "Direction.generated.h"

/*
 *
 * 
 */
UENUM(BlueprintType)
enum class EDirection:uint8
{
	ED_Up UMETA(DisplayName = "Up"),
    ED_Down UMETA(DisplayName = "Down"),
    ED_Right UMETA(DisplayName = "Right"),
    ED_Left UMETA(DisplayName = "Left")
};