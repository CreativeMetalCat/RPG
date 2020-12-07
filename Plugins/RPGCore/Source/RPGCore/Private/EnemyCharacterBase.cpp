// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacterBase.h"



#include "AIController.h"
#include "Player/PlayerBase.h"

AEnemyCharacterBase::AEnemyCharacterBase()
{
    SenseSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SenseSphere"));
    SenseSphereComponent->SetupAttachment(RootComponent);//Attach to RootComponent
    SenseSphereComponent->InitSphereRadius(2000.f);//Basic radius for the sphere. Can be changed in Blueprints 
}

void AEnemyCharacterBase::UpdateAI()
{
    if(GetController() != nullptr)//no point in doing ai checks if it's not an ai
    {
        if(AAIController* NPC = Cast<AAIController>(GetController()))
        {
            TArray<AActor*> SensedActors;//Actors currently senses by ai
            SenseSphereComponent->GetOverlappingActors(SensedActors,ARPGCharacterBase::StaticClass());
            if (SensedActors.Find(CurrentTarget) != INDEX_NONE)
            {
                //do stuff if target is still visible/in range
            }
            else if (SensedActors.Num() > 0) //find target, but only if we sense anyone
            {
                //for now it's simple check if player and set is as target
                //TODO: Add more complex system to allows AIs battle each other
                for (int i = 0; i < SensedActors.Num(); i++)
                {
                    if (APlayerBase* Player = Cast<APlayerBase>(SensedActors[i]))//not effective, does a lot of casting TODO: Replace with Class checks
                    {
                        if (!Player->bDead)
                        {
                            CurrentTarget = Player;

                            if (GetController()->Implements<UAIInterface>() && Cast<IAIInterface>(GetController()))
                            {
                               IAIInterface::Execute_SetNewTarget(GetController(),CurrentTarget);/*This way we don't need to cast to AIBase class*/ 
                            }
                            return;
                        }
                    }
                }
                /*We didn't find anyone who fits description so we just clear the target*/
                IAIInterface::Execute_SetNewTarget(GetController(),nullptr);/*passing nullptr clears it*/
            }
        }
    }
}

void AEnemyCharacterBase::BeginPlay()
{
    Super::BeginPlay();

    GetWorldTimerManager().SetTimer(AIUpdateHandle,this,&AEnemyCharacterBase::UpdateAI,AIUpdateRate,true);
}
