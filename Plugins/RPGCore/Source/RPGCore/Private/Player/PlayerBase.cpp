// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGCore/Public/Player/PlayerBase.h"


#include "Audio/AudioCaptionFunctionLibrary.h"
#include "Player/PlayerLevelInfo.h"

void APlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
   	
    PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerBase::InputAttack);

    PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerBase::InteractAction);

	PlayerInputComponent->BindAction("DodgeRoll", IE_Pressed, this, &APlayerBase::DodgeRoll);
	
#pragma region AbilitiesInputFunctions
	PlayerInputComponent->BindAction("UseAbility1", IE_Pressed, this, &APlayerBase::UseAbility1);
	PlayerInputComponent->BindAction("UseAbility2", IE_Pressed, this, &APlayerBase::UseAbility2);
	PlayerInputComponent->BindAction("UseAbility3", IE_Pressed, this, &APlayerBase::UseAbility3);
	PlayerInputComponent->BindAction("UseAbility4", IE_Pressed, this, &APlayerBase::UseAbility4);

	PlayerInputComponent->BindAction("UseAbility1", IE_Released, this, &APlayerBase::ReleaseAbilitySlot1Button);
	PlayerInputComponent->BindAction("UseAbility2", IE_Released, this, &APlayerBase::ReleaseAbilitySlot2Button);
	PlayerInputComponent->BindAction("UseAbility3", IE_Released, this, &APlayerBase::ReleaseAbilitySlot3Button);
	PlayerInputComponent->BindAction("UseAbility4", IE_Released, this, &APlayerBase::ReleaseAbilitySlot4Button);
#pragma endregion
	
    PlayerInputComponent->BindAxis("MoveRight", this, &APlayerBase::MoveRight);
    PlayerInputComponent->BindAxis("MoveUp", this, &APlayerBase::MoveUp);
}

void APlayerBase::BindEventOnAbilityButtonReleased_Implementation(ASpecialEffect* Effect)
{
	switch (GetAbilitySlotForAbility(Effect->AbilityId))
	{
	case 0:
		OnAbilitySlot1ButtonReleased.AddDynamic(Effect,&ASpecialEffect::OnAbilityButtonReleased);
		break;
	case 1:
		OnAbilitySlot2ButtonReleased.AddDynamic(Effect,&ASpecialEffect::OnAbilityButtonReleased);
		break;
	case 2:
		OnAbilitySlot3ButtonReleased.AddDynamic(Effect,&ASpecialEffect::OnAbilityButtonReleased);
		break;
	case 3:
		OnAbilitySlot4ButtonReleased.AddDynamic(Effect,&ASpecialEffect::OnAbilityButtonReleased);
		break;
	default:
		//ability is out of range. so there is nothing we can do
		break;
	}
}

void APlayerBase::ApplyComboBonus_Implementation()
{
	//clear timer and count to avoid bugs with counting
	if(ComboAttackResetTimerHandle.IsValid()){ComboAttackResetTimerHandle.Invalidate();}
	ComboAttackCurrentCount = 0;

	//player reacts to successful combo by reducing the remaining time on abilities' cooldown
	if(Abilities.Num() > 0)
	{
		float RemainingTime = 0;
		for (int i = 0; i < Abilities.Num(); i++)
		{
			if(Abilities[i].bIsCoolingdown)
			{
				RemainingTime = GetWorldTimerManager().GetTimerRemaining(Abilities[i].CooldownTimerHandle);
				if((RemainingTime - ComboTimeBonus) > 0)
				{
					//because UE4 doesn't allow to directly change timer's remaining time
					//we will just clear it and create new on with updated time
					GetWorldTimerManager().ClearTimer(Abilities[i].CooldownTimerHandle);
					GetWorldTimerManager().SetTimer(Abilities[i].CooldownTimerHandle,
                                                FTimerDelegate::CreateUObject
                                                (
                                                    this,
                                                    &ARPGCharacterBase::FinishCooldownOnAbility,
                                                    Abilities[i].DevName
                                                ),
                                                (RemainingTime - ComboTimeBonus),false);
				}
				else
				{
					//because UE4 doesn't allow to directly change timer's remaining time
					//we will just clear it and create new on with updated time
					//but because timer with 0 remaining time will not be created(and as such funtion will not be executed)
					//we have to set a value bigger then 0
					GetWorldTimerManager().ClearTimer(Abilities[i].CooldownTimerHandle);
					GetWorldTimerManager().SetTimer(Abilities[i].CooldownTimerHandle,
                                                FTimerDelegate::CreateUObject
                                                (
                                                    this,
                                                    &ARPGCharacterBase::FinishCooldownOnAbility,
                                                    Abilities[i].DevName
                                                ),
                                                0.01f,false);
				}
			}
		}
	}
}

void APlayerBase::Die_Implementation()
{
	Super::Die();
	
	if(DeathSound != nullptr)//check validity of the sound before playing
	{
		//Create caption info
		FAudioInfo info = FAudioInfo();
		info.bIsDirectional = false;
		info.SoundName = "Player dies";
		info.SoundLength = DeathSound->Duration;
		
		UAudioCaptionFunctionLibrary::PlaySoundWithCaption(this,DeathSound,info,GetActorLocation(),GetActorRotation());
	}
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

void APlayerBase::DodgeRoll()
{
	Roll(CurrentDirection);
}

void APlayerBase::UseAbilityInSlot(int slotId)
{
	switch (slotId)
	{
	case 1:
		UseAbility(Slot1AbilityId);
		break;
	case 2:
		UseAbility(Slot2AbilityId);
		break;
	case 3:
		UseAbility(Slot3AbilityId);
		break;
	case 4:
		UseAbility(Slot4AbilityId);
		break;
	default: break;
	}
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
	int neededExp = 0;
	if(PlayerLevelInfoTable)
	{
		if(PlayerLevelInfoTable->GetRowNames().IsValidIndex(Level))
		{
			FPlayerLevelInfo info = *(PlayerLevelInfoTable->FindRow<FPlayerLevelInfo>(PlayerLevelInfoTable->GetRowNames()[Level],""));
			neededExp = info.NeededExp;
		}
		else
		{
			//if row wasn't found do it this way
			neededExp = (Level + 1) * NeededExperienceMultiplier;
		}
	}
	else
	{
		//if row wasn't found do it this way
		neededExp = (Level + 1) * NeededExperienceMultiplier;
	}
	
	if(Experience >= neededExp)
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

	
	if (Quests[questId].bCanAffectOtherQuests)
	{
		TArray<FString> AffectedQuests;
		TArray<int> AffectedQuestsProgressAmount;

		Quests[questId].QuestsToAffect.GenerateValueArray(AffectedQuestsProgressAmount);
		Quests[questId].QuestsToAffect.GetKeys(AffectedQuests);

		if (AffectedQuests.Num() > 0)
		{
			for (int i = 0; i < AffectedQuests.Num(); i++)
			{
				MakeProgress(AffectedQuests[i], AffectedQuestsProgressAmount[i]);
			}
		}
	}
	
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

	if(PlayerLevelInfoTable)
	{
		if(PlayerLevelInfoTable->GetRowNames().IsValidIndex(Level))
		{
			FPlayerLevelInfo info = *(PlayerLevelInfoTable->FindRow<FPlayerLevelInfo>(PlayerLevelInfoTable->GetRowNames()[Level],""));
			SkillPoints += info.SkillPoints;
		}
		else
		{
			//we reached last possible level or table is null so we just give random points
			//TODO: Create better last level solution before release
			SkillPoints += FMath::RandRange(0,5);
		}
	}
	else
	{
		//we reached last possible level or table is null so we just give random points
		//TODO: Create better last level solution before release
		SkillPoints += FMath::RandRange(0,5);
	}
	
	UpdatePlayerInfo();

	if (Experience >= Level * NeededExperienceMultiplier) { LevelUp(); }
}