// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGCore/Public/Item/SpecialEffect.h"

void ASpecialEffect::ApplyEffect_Implementation(AActor* CreatorOfEffect, FVector Origin,UWorld *WorldContext,AActor*TargetActor )
{
    Creator = CreatorOfEffect;
    EffectOrigin = Origin;
    Target = TargetActor;
    
    if(bDoesEffectLasts)
    {
        if(WorldContext)
        {
           WorldContext->GetTimerManager().SetTimer(EffectTimer,this,&ASpecialEffect::OnEffectEnds,EffectLifeSpan);
           WorldContext->GetTimerManager().SetTimer(EffectLoopTimer,this,&ASpecialEffect::OnLoop,EffectLoopTime,true);
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,"Error: World Context is null");
        }
    }
}

void ASpecialEffect::OnEffectEnds_Implementation()
{
    Destroy();
}

void ASpecialEffect::OnLoop_Implementation()
{
    
}
