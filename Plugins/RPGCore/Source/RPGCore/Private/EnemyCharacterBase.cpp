// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacterBase.h"



#include "AIController.h"
#include "Player/PlayerBase.h"

AEnemyCharacterBase::AEnemyCharacterBase()
{
    SenseSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SenseSphere"));
    SenseSphereComponent->SetupAttachment(RootComponent);//Attach to RootComponent
    SenseSphereComponent->InitSphereRadius(2000.f);//Basic radius for the sphere. Can be changed in Blueprints


    UpperCollision->OnComponentBeginOverlap.AddDynamic(this,& AEnemyCharacterBase::OnAttackCollisionOverlapBegin);

    LowerCollision->OnComponentBeginOverlap.AddDynamic(this,& AEnemyCharacterBase::OnAttackCollisionOverlapBegin);

    LeftCollision->OnComponentBeginOverlap.AddDynamic(this,& AEnemyCharacterBase::OnAttackCollisionOverlapBegin);

    RightCollision->OnComponentBeginOverlap.AddDynamic(this,& AEnemyCharacterBase::OnAttackCollisionOverlapBegin);
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

}

TArray<FName> AEnemyCharacterBase::GetEnemyTags_Implementation()
{
    return EnemyTags;
}

void AEnemyCharacterBase::GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const
{
    Location = GetActorLocation();

    switch (CurrentDirection)
    {
        case EDirection::ED_Down:
            
            break;
    }
}

void AEnemyCharacterBase::OnAttackCollisionOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if(OverlappedComp == UpperCollision)
    {
        GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::Red,"Attackinng UP");
        CurrentDirection = EDirection::ED_Up;
        Attack(AttackPower);
    }
    else if(OverlappedComp == LowerCollision)
    {
        GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::Red,"Attackinng DOWN");
        CurrentDirection = EDirection::ED_Down;
        Attack(AttackPower);
    }
    else if(OverlappedComp == LeftCollision)
    {
        GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::Red,"Attackinng LEFT");
        CurrentDirection = EDirection::ED_Left;
        Attack(AttackPower);
    }
    else if(OverlappedComp == RightCollision)
    {
        GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::Red,"Attackinng RIGHT");
        CurrentDirection = EDirection::ED_Right;
        Attack(AttackPower);
    }
    GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::Red,"Collision");
}
