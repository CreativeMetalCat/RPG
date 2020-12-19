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
        Rotation = FRotator(0,0,90);
        break;
    case EDirection::ED_Up:
        Rotation = FRotator(0,0,270);
        break;
    case EDirection::ED_Right:
        Rotation = FRotator(0,0,0);
        break;
    case EDirection::ED_Left:
        Rotation = FRotator(0,0,180);
        break;
    default:
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
