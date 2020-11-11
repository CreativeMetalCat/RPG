// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGCore/Public/Player/PlayerBase.h"

void APlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
   	
    PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerBase::Attack);

    PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerBase::InteractAction);
	
    PlayerInputComponent->BindAxis("MoveRight", this, &APlayerBase::MoveRight);
    PlayerInputComponent->BindAxis("MoveUp", this, &APlayerBase::MoveUp);
}


APlayerBase::APlayerBase()
{
	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 0.0f);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->SetRelativeRotation(FRotator(180.0f, -90.0f, 180.0f));
	

	// Create an orthographic camera (no perspective) and attach it to the boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCameraComponent->OrthoWidth = 2048.0f;
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Prevent all automatic rotation behavior on the camera, character, and camera component
	CameraBoom->SetUsingAbsoluteRotation(true);
	SideViewCameraComponent->bUsePawnControlRotation = false;
	SideViewCameraComponent->bAutoActivate = true;
}

void APlayerBase::MoveRight(float Value)
{
	/*UpdateChar();*/
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		AddMovementInput(FVector(1,0,0), Value);
	}
}

void APlayerBase::MoveUp(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// add movement in that direction
		AddMovementInput(FVector(0,1,0), Value);
	}
}

void APlayerBase::AddExp_Implementation(int amount)
{
	Experience += amount;
	
	if(Experience >= (Level + 1) * NeededExperienceMultiplier)
	{
		LevelUp();
	}
	UpdatePlayerInfo();
}


FQuestInfo APlayerBase::GetQuestInfo(FString devName,bool&hasQuest)
{
	hasQuest = false;
	if(Quests.Num() > 0)
	{
		for(int i=0;i<Quests.Num();i++)
		{
			if(Quests[i].DevName == devName)
			{
				hasQuest = Quests[i].bShow;
				return Quests[i];
			}			
		}
	}
	return FQuestInfo();
}

bool APlayerBase::HasQuest(const FString& devName)
{
	if(Quests.Num() > 0)
	{
		for(int i=0;i<Quests.Num();i++)
		{
			if(Quests[i].DevName == devName){return Quests[i].bShow;}			
		}		
	}
	return false;
}


bool APlayerBase::AddQuest_Implementation(FQuestInfo quest)
{
	if(Quests.Num() > 0)
	{
		for(int i=0;i<Quests.Num();i++)
		{
			if(Quests[i].DevName == quest.DevName)
			{
				if(Quests[i].bShow)
				{
					return false;
				}
				else
				{
					Quests[i].bShow = true;
					if(Quests[i].CurrentProgress >= Quests[i].NeededProgress)
					{
						CompleteQuest(Quests[i].DevName,i);
					}
					UpdateQuestDisplayInfo();
					return true;
				}
			}			
		}
	}
	
	quest.bShow = true;
	Quests.Add(quest);
	UpdateQuestDisplayInfo();
	return true;
}


bool APlayerBase::AddQuestToQuestArray(FQuestInfo quest)
{
	if(Quests.Num() > 0)
	{
		for(int i=0;i<Quests.Num();i++)
		{
			if(Quests[i].DevName == quest.DevName)
			{
				return false;
			}			
		}
		Quests.Add(quest);
		UpdateQuestDisplayInfo();
		return true;
	}
	else
	{
		Quests.Add(quest);
		UpdateQuestDisplayInfo();
		return true;
	}
}

void APlayerBase::MarkQuestAsRewarded_Implementation(const FString& devName)
{
	if(Quests.Num() > 0)
	{
		for(int i=0;i < Quests.Num(); i++)
		{
			if(Quests[i].DevName == devName)
			{
				Quests[i].bRewarded = true;
				return;
			}
		}
	}
}

bool APlayerBase::CompleteQuest_Implementation(const FString& devName,int questId)
{
	if(Quests[questId].bCompleted){return false;}
	
	Quests[questId].bCompleted = true;
	OnQuestFinished.Broadcast(Quests[questId]);
	return true;
}

void APlayerBase::MakeProgress_Implementation(const FString& devName, int amount)
{
	if(Quests.Num() > 0)
	{
		for(int i=0;i < Quests.Num(); i++)
		{
			if(Quests[i].DevName == devName)
			{
				if(Quests[i].bShow || Quests[i].bAllowProgressBeforeGiven)
				{
					Quests[i].CurrentProgress += amount;
					if(Quests[i].bShow && Quests[i].CurrentProgress >= Quests[i].NeededProgress)
					{
						CompleteQuest(Quests[i].DevName,i);
					}
					UpdateQuestDisplayInfo();
					break;
				}
				else
				{
					break;
				}
			}
		}
	}
}

void APlayerBase::ChangeQuestInfo_Implementation(FQuestInfo newQuestInfo)
{
	if(Quests.Num() > 0)
	{
		for(int i=0;i < Quests.Num(); i++)
		{
			if(Quests[i].DevName == newQuestInfo.DevName)
			{
				Quests[i] = newQuestInfo;
				break;
			}
		}
	}
	UpdateQuestDisplayInfo();
}

void APlayerBase::UpdateQuestDisplayInfo_Implementation()
{
}

void APlayerBase::LevelUp_Implementation()
{
	Level++;

	Experience = (Experience-Level*NeededExperienceMultiplier <= 0)? 0: Experience-Level*NeededExperienceMultiplier;	
	
	SkillPoints += FMath::RandRange(0,5);
	UpdatePlayerInfo();

	if (Experience >= Level * NeededExperienceMultiplier) { LevelUp(); }
}