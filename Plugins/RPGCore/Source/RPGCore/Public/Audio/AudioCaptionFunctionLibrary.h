// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AudioInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "AudioCaptionFunctionLibrary.generated.h"


/**
 * 
 */
UCLASS()
class RPGCORE_API UAudioCaptionFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	public:
	/*Displays caption for specified time
	* Player -> Actor responsible for displaying the caption, must implement IAccessibilityInterface
	* If it doesn't or you don't need captions don't use this function at all
	*/
	UFUNCTION(BlueprintCallable,Category="AudioSystem|OneTimeSound", meta=(WorldContext="WorldContextObject", UnsafeDuringActorConstruction = "true", Keywords = "caption"))
	static void AddSoundCaption(AActor*player,FAudioInfo info);

	/*Plays sounds Using simple "Play sound at location" function
	 * Player -> Actor responsible for displaying the caption, must implement IAccessibilityInterface
	 * If it doesn't or you don't need captions use default function
	 */
	UFUNCTION(BlueprintCallable,Category="AudioSystem|OneTimeSound", meta=(WorldContext="WorldContextObject", AdvancedDisplay = "3", UnsafeDuringActorConstruction = "true", Keywords = "play"))
	static void PlaySoundWithCaption(AActor* player, USoundBase* Sound, FAudioInfo info,
	                                 FVector Location, FRotator Rotation,
	                                 float VolumeMultiplier = 1.f, float PitchMultiplier= 1.f, float StartTime= 0.f,
	                                 class USoundAttenuation* AttenuationSettings = nullptr,
	                                 USoundConcurrency* ConcurrencySettings = nullptr);
};
