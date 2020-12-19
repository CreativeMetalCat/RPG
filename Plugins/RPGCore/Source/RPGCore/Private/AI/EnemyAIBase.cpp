// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGCore/Public/AI/EnemyAIBase.h"

#include "EnemyCharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/TargetPoint.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"

AEnemyAIBase::AEnemyAIBase()
{
   AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));
   
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

void AEnemyAIBase::Update()
{
   if(GetPawn())
   {
      if(Cast<IAIInterface>(GetPawn()) || GetPawn()->Implements<UAIInterface>())
      {
         //Actors currently seen by ai
         TArray<AActor*> SensedActors;
         AIPerceptionComponent->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(),SensedActors);

         /*The idea of how it works
          * IT checks if we still see player -> if we don't we start checking if anyone we see can be a target ->
          * -> if we see someone who fits we set it as target -> finish execution
          * -> if we don't or just don't see anyone -> clear target -> finish the execution
          */
         if (SensedActors.Find(CurrentTarget) != INDEX_NONE)
         {
            //do stuff if target is still visible/in range

            //record last seen location, but don't set it yet
            LastSeenLocation = CurrentTarget->GetActorLocation();
            return;
         }
         else if (SensedActors.Num() > 0) //find target, but only if we sense anyone
         {
            for (int i = 0; i < SensedActors.Num(); i++)
            {
               if (IsEnemy(SensedActors[i]))
               {
                  SetTarget(SensedActors[i]);
                  return;
               }
            }        
         }
         
         if(Blackboard && CurrentTarget)//set only if we saw target recently
         {
            LastSeenLocation = CurrentTarget->GetActorLocation();
            Blackboard->SetValueAsVector(LastSeenLocationName,LastSeenLocation);
         }
         
         //Just Clear target if we didn't find anyone who fits
         SetTarget(nullptr);
      }
   }
}

void AEnemyAIBase::ResetLastSeenLocation()
{
   //Resetting is simple -> just clear value and that's it
   if(Blackboard)
   {
      Blackboard->ClearValue(LastSeenLocationName);
   }
}

bool AEnemyAIBase::IsEnemy(AActor* Actor)
{
   if(Actor)
   {
      for (int i = 0; i < EnemyTags.Num(); i++)
      {
         if(Actor->Tags.Find(EnemyTags[i]) != INDEX_NONE)
         {
            if(ARPGCharacterBase * character = Cast<ARPGCharacterBase>(Actor))
            {
               return !character->bDead;
            }
            else
            {
               return true;
            }
         }
      }
   }
   return false;
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

         /*Setup ai hatred*/
         if(Cast<IAIInterface>(GetPawn()) || GetPawn()->Implements<UAIInterface>())
         {
           EnemyTags =  IAIInterface::Execute_GetEnemyTags(GetPawn());
         }

         //setup AI update timer
         GetWorldTimerManager().SetTimer(AIUpdateTimerHandle,this,&AEnemyAIBase::Update,AIUpdateRate,true);

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
   SetTarget(Target);
}

void AEnemyAIBase::SetTarget(AActor* Target)
{
   CurrentTarget = Target;
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

