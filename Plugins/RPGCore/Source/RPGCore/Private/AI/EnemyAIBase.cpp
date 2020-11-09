// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGCore/Public/AI/EnemyAIBase.h"

#include "EnemyCharacterBase.h"

void AEnemyAIBase::OnReachedGoalOfRandomMovement()
{
   //to imitate ai thinking
   GetWorldTimerManager().SetTimer(RandomMovementTimerHandle,this,&AEnemyAIBase::SetNextGoalForRandomMovement,RandomMovementTime);
}

void AEnemyAIBase::SetNextGoalForRandomMovement()
{
   if(GetPawn())
   {
      FVector2D Rand = FMath::RandPointInCircle(MaxDistanceOfRandomMovement);
      RandomMovementGoal = (bGenerateRandomPointsFromDefaultLocation ? SpawnLocation :GetPawn()->GetActorLocation()) + FVector(Rand.X,Rand.Y,0);
      MoveToLocation(RandomMovementGoal);
      GetWorldTimerManager().SetTimer(RandomMovementTimerHandle,this,&AEnemyAIBase::OnReachedGoalOfRandomMovement,RandomMovementTime);
   } 
}

void AEnemyAIBase::BeginPlay()
{
   Super::BeginPlay();
   if(GetPawn())
   {
      SpawnLocation = GetPawn()->GetActorLocation();
      
      if(AEnemyCharacterBase* Enemy = Cast<AEnemyCharacterBase>(GetPawn()))
      {
         MaxDistanceOfRandomMovement = Enemy->MaxDistanceOfRandomMovement;
         RandomMovementTime = Enemy->RandomMovementTime;
         bGenerateRandomPointsFromDefaultLocation = Enemy->bGenerateRandomPointsFromDefaultLocation;
         MovementType = Enemy->MovementType;
      }
      else
      {
         GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
                                          "AIController " + GetName() + " is not used with correct class. Class: " +
                                          GetPawn()->GetClass()->GetName() + " Correct is child of: " +
                                          AEnemyCharacterBase::StaticClass()->GetName());
      }
   
      if(MovementType == EAIMovementType::EAIMT_Random)
      {     
         SetNextGoalForRandomMovement();
      }
   }
   else
   {
      GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,"AIController " + GetName() + " has null Pawn in control");
   }
}
