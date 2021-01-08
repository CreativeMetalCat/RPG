// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "AudioInfo.generated.h"

/*This struct describes all the info needed for audio caption system*/
USTRUCT(BlueprintType)
struct FAudioInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString SoundName;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsDirectional;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector Location;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float SoundLength;
};
