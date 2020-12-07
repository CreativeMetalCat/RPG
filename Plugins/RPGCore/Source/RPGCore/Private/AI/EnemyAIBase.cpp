// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGCore/Public/AI/EnemyAIBase.h"

#include "EnemyCharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/TargetPoint.h"

AEnemyAIBase::AEnemyAIBase()
{
   
}

void AEnemyAIBase::SetNewPatrolPoint()
{
   if(Blackboard)
   {
      if(PatrolPoints.Num() > 0)
      {
         if(bRandomPatrol)
         {
            CurrentPatrolPointId = FMath::RandRange(0,PatrolPoints.Num());

            Blackboard->SetValueAsObject(BlackboardPatrolPointTargetName,PatrolPoints[CurrentPatrolPointId]);
         }
         else
         {
            CurrentPatrolPointId++;
            if(CurrentPatrolPointId >= PatrolPoints.Num()){CurrentPatrolPointId = 0;}
            Blackboard->SetValueAsObject(BlackboardPatrolPointTargetName,PatrolPoints[CurrentPatrolPointId]);
         }
      }
      Blackboard->SetValueAsBool(BlackboardWaitingName,false);
   }
   else
   {
      GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
                                       "AIController " + GetName() +
                                       " has no Behaviour Tree setup. Patrol System needs behaviour tree to function");
   }
}

void AEnemyAIBase::OnReachedPatrolPoint()
{
   if(PatrolPoints.IsValidIndex(CurrentPatrolPointId))
   {
      if(PatrolPoints[CurrentPatrolPointId]->WaitTime)
      {
         //to imitate ai thinking
         GetWorldTimerManager().SetTimer(RandomMovementTimerHandle,this,&AEnemyAIBase::SetNewPatrolPoint,PatrolPoints[CurrentPatrolPointId]->WaitTime);
         Blackboard->SetValueAsBool(BlackboardWaitingName,true);
         return;//break execution flow
      }
   }
   SetNewPatrolPoint();
}

void AEnemyAIBase::OnReachedGoalOfRandomMovement()
{
   if(RandomMovementTime > 0)
   {
      //to imitate ai thinking
      GetWorldTimerManager().SetTimer(RandomMovementTimerHandle,this,&AEnemyAIBase::SetNextGoalForRandomMovement,RandomMovementTime);
   }
   else
   {
      SetNextGoalForRandomMovement();
   }
}

void AEnemyAIBase::SetNextGoalForRandomMovement()
{
   if(GetPawn())
   {
      FVector2D Rand = FMath::RandPointInCircle(MaxDistanceOfRandomMovement);
      RandomMovementGoal = (bGenerateRandomPointsFromDefaultLocation ? SpawnLocation :GetPawn()->GetActorLocation()) + FVector(Rand.X,Rand.Y,0);
     // MoveToLocation(RandomMovementGoal);/*Old solution*/
      RandomMovementGoalLocationActor->SetActorLocation(RandomMovementGoal);
      if(!bRandomGoalActorIsSet && Blackboard)
      {
         Blackboard->SetValueAsObject(BlackboardPatrolPointTargetName,RandomMovementGoalLocationActor);
         bRandomGoalActorIsSet = true;
      }
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
         MovementType = Enemy->MovementType;
         
         /*Setup random movement*/
         MaxDistanceOfRandomMovement = Enemy->MaxDistanceOfRandomMovement;
         RandomMovementTime = Enemy->RandomMovementTime;
         bGenerateRandomPointsFromDefaultLocation = Enemy->bGenerateRandomPointsFromDefaultLocation;
         
         /*Setup patrol movement*/
         PatrolPoints = Enemy->PatrolPoints;
         bRandomPatrol = Enemy->bRandomPatrol;

         GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,FString::FromInt(Enemy->PatrolPoints.Num()));
      }
      else
      {
         GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
                                          "AIController " + GetName() + " is not used with correct class. Class: " +
                                          GetPawn()->GetClass()->GetName() + " Correct is child of: " +
                                          AEnemyCharacterBase::StaticClass()->GetName());
         return;
      }
   
      if(MovementType == EAIMovementType::EAIMT_Random)
      {
         RandomMovementGoalLocationActor = GetWorld()->SpawnActor(ATargetPoint::StaticClass());
         SetNextGoalForRandomMovement();
      }
      else if(MovementType == EAIMovementType::EAIMT_Points)
      {
         SetNewPatrolPoint();
      }

   }
   else
   {
      GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,"AIController " + GetName() + " has null Pawn in control");
      return;
   }

}

void AEnemyAIBase::SetNewTarget_Implementation(AActor* Target)
{
   if(Blackboard)
   {
      if(Target != nullptr)
      {
         Blackboard->SetValueAsObject(BlackboardTargetName,Target);
      }
      else
      {
         Blackboard->ClearValue(BlackboardTargetName);
      }
   }
}

