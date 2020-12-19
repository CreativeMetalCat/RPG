// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacterBase.h"



#include "AIController.h"
#include "Player/PlayerBase.h"

//Because it's stupid name for a macro so only this project should have it
#ifdef CHECK_AND_ATTACK
#undef CHECK_AND_ATTACK
#endif //CHECK_AND_ATTACK
//Simple macro that checks if target is in trigger and attacks
#define CHECK_AND_ATTACK(collisionName,direction){\
        TArray<AActor*> overlapped;\
        collisionName##Collision->GetOverlappingActors(overlapped);\
        if(overlapped.Find(CurrentTarget) != INDEX_NONE)\
        {\
            CurrentDirection = EDirection::ED_##direction;\
            Attack(AttackPower);\
        }\
}\


AEnemyCharacterBase::AEnemyCharacterBase()
{
    SenseSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SenseSphere"));
    SenseSphereComponent->SetupAttachment(RootComponent);//Attach to RootComponent
    SenseSphereComponent->InitSphereRadius(2000.f);//Basic radius for the sphere. Can be changed in Blueprints


    UpperCollision->OnComponentBeginOverlap.AddDynamic(this,& AEnemyCharacterBase::OnAttackCollisionOverlapBegin);

    LowerCollision->OnComponentBeginOverlap.AddDynamic(this,& AEnemyCharacterBase::OnAttackCollisionOverlapBegin);

    LeftCollision->OnComponentBeginOverlap.AddDynamic(this,& AEnemyCharacterBase::OnAttackCollisionOverlapBegin);

    RightCollision->OnComponentBeginOverlap.AddDynamic(this,& AEnemyCharacterBase::OnAttackCollisionOverlapBegin);

    UpperCollision->OnComponentEndOverlap.AddDynamic(this,&AEnemyCharacterBase::OnAttackCollisionEndOverlap);

    LowerCollision->OnComponentEndOverlap.AddDynamic(this,&AEnemyCharacterBase::OnAttackCollisionEndOverlap);

    LeftCollision->OnComponentEndOverlap.AddDynamic(this,&AEnemyCharacterBase::OnAttackCollisionEndOverlap);

    RightCollision->OnComponentEndOverlap.AddDynamic(this,&AEnemyCharacterBase::OnAttackCollisionEndOverlap);
}


void AEnemyCharacterBase::BeginPlay()
{
    Super::BeginPlay();
    
    if(Cast<IAIInterface>(GetController()) || GetController()->Implements<UAIInterface>())
    {
        bIsControllerCompatible = true;
    }
    
    GetWorldTimerManager().SetTimer(AttackCollisionCheckUpdateTimerHandle,this,&AEnemyCharacterBase::CheckAttackCollision,AttackCheckUpdateRate,true);
}

void AEnemyCharacterBase::CheckAttackCollision()
{
    //because we can only attack one enemy at once, it will simply attack the first one in line
    GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::Magenta,CurrentTarget == nullptr? "Valid": "Invalid");
    if (CurrentTarget)
    {
        if (bAnythingOverlappingUpperCollision)
        {
            CHECK_AND_ATTACK(Upper, Up);
            return;
        }
        if (bAnythingOverlappingRightCollision)
        {
            CHECK_AND_ATTACK(Right, Right);
            GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::Magenta,"Text");
            return;
        }
        if (bAnythingOverlappingLowerCollision)
        {
            CHECK_AND_ATTACK(Lower, Down);
            return;
        }
        if (bAnythingOverlappingLeftCollision)
        {
            CHECK_AND_ATTACK(Left, Left);
            return;
        }
    }
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

void AEnemyCharacterBase::SetNewTarget_Implementation(AActor* Target)
{
    CurrentTarget = Target;
}

void AEnemyCharacterBase::OnAttackCollisionOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    
    if(OverlappedComp == UpperCollision)
    {
        CurrentDirection = EDirection::ED_Up;
       bAnythingOverlappingUpperCollision = true;
    }
    else if(OverlappedComp == LowerCollision)
    {
        CurrentDirection = EDirection::ED_Down;
        bAnythingOverlappingLowerCollision = true;
    }
    else if(OverlappedComp == LeftCollision)
    {
        CurrentDirection = EDirection::ED_Left;
        bAnythingOverlappingLeftCollision = true;
    }
    else if(OverlappedComp == RightCollision)
    {
        CurrentDirection = EDirection::ED_Right;
        bAnythingOverlappingRightCollision = true;
    }
}

void AEnemyCharacterBase::OnAttackCollisionEndOverlap(UPrimitiveComponent* OverlappedComp,AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
    if (OverlappedComp == UpperCollision)
    {
        TArray<AActor*> Overlapped;
        OverlappedComp->GetOverlappingActors(Overlapped);
        if(Overlapped.Num() == 0)
        {
            bAnythingOverlappingUpperCollision = false;
        }
        return;
    }
    else if (OverlappedComp == LowerCollision)
    {
        TArray<AActor*> Overlapped;
        OverlappedComp->GetOverlappingActors(Overlapped);
        if(Overlapped.Num() == 0)
        {
            bAnythingOverlappingLowerCollision = false;
        }
        return;
    }
    else if (OverlappedComp == LeftCollision)
    {
        TArray<AActor*> Overlapped;
        OverlappedComp->GetOverlappingActors(Overlapped);
        if(Overlapped.Num() == 0)
        {     
            bAnythingOverlappingLeftCollision = false;
        }
        return;
    }
    else if (OverlappedComp == RightCollision)
    {
        TArray<AActor*> Overlapped;
        OverlappedComp->GetOverlappingActors(Overlapped);
        if(Overlapped.Num() == 0)
        {
            bAnythingOverlappingRightCollision = false;
        }
        return;      
    }
}
