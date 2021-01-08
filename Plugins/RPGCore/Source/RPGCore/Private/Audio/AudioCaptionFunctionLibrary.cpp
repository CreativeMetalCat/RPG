// Fill out your copyright notice in the Description page of Project Settings.


#include "Audio/AudioCaptionFunctionLibrary.h"

#include "AccessibilityInterface.h"
#include "Kismet/GameplayStatics.h"


void UAudioCaptionFunctionLibrary::AddSoundCaption(AActor*player,FAudioInfo info)
{
	if(player)
	{
		if(Cast< IAccessibilityInterface>(player) || player->Implements<UAccessibilityInterface>())
		{
			IAccessibilityInterface::Execute_AddNewSoundCaption(player,info);
		}
	}
}

void UAudioCaptionFunctionLibrary::PlaySoundWithCaption(AActor* player, USoundBase* Sound, FAudioInfo info,
                                                        FVector Location, FRotator Rotation,
                                                        float VolumeMultiplier, float PitchMultiplier, float StartTime,
                                                        class USoundAttenuation* AttenuationSettings,
                                                        USoundConcurrency* ConcurrencySettings)
{
	if(player && Sound)
	{
		if(info.bIsDirectional)
		{
			UGameplayStatics::PlaySoundAtLocation(player->GetWorld(), Sound, Location, Rotation, VolumeMultiplier,
                                              PitchMultiplier, StartTime, AttenuationSettings, ConcurrencySettings);
		}
		else
		{
			UGameplayStatics::PlaySound2D(player->GetWorld(), Sound,VolumeMultiplier,
                                              PitchMultiplier, StartTime, ConcurrencySettings);
		}
			
		if(Cast< IAccessibilityInterface>(player) || player->Implements<UAccessibilityInterface>())
		{
			IAccessibilityInterface::Execute_AddNewSoundCaption(player,info);
		}
	}
}
