// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGCore/Public/RPGCharacterBase.h"

#include "RPGCore/Public/Interaction.h"
#include "PaperFlipbookComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

/*
* if(ArmorTopPartDisplayFlipbookComponent->GetFlipbook() != ArmorTopPartInfo.GetAttackAnimation(CurrentDirection))
{
ArmorTopPartDisplayFlipbookComponent->SetFlipbook(ArmorTopPartInfo.GetAttackAnimation(CurrentDirection));
}
ArmorTopPartDisplayFlipbookComponent->SetLooping(false);
 */
#define PlAY_ANIMATION_ON_ITEM(itemName,animName,loop) {\
	if(itemName##DisplayFlipbookComponent->GetFlipbook()!= itemName##Info.Get##animName##Animation(CurrentDirection))\
	{\
	itemName##DisplayFlipbookComponent->SetFlipbook(itemName##Info.Get##animName##Animation(CurrentDirection));\
	}\
	itemName##DisplayFlipbookComponent->SetLooping(loop);\
	itemName##DisplayFlipbookComponent->Play();\
	}

#define PlAY_SPECIAL_ANIMATION_ON_ITEM(itemName,animName,loop) {\
if(itemName##DisplayFlipbookComponent->GetFlipbook()!= itemName##Info.GetSpecialAnimation(animName,CurrentDirection))\
{\
itemName##DisplayFlipbookComponent->SetFlipbook(itemName##Info.GetSpecialAnimation(animName,CurrentDirection));\
}\
itemName##DisplayFlipbookComponent->SetLooping(loop);\
itemName##DisplayFlipbookComponent->Play();\
}

#define PlAY_ANIMATION_ON_ITEM_WITH_SELECTION(itemName,animName1,animName2,condition,loop) {\
	if(itemName##DisplayFlipbookComponent->GetFlipbook()!= (condition?itemName##Info.Get##animName1##Animation(CurrentDirection):itemName##Info.Get##animName2##Animation(CurrentDirection)))\
	{\
		itemName##DisplayFlipbookComponent->SetFlipbook(condition?itemName##Info.Get##animName1##Animation(CurrentDirection):itemName##Info.Get##animName2##Animation(CurrentDirection));\
	}\
	itemName##DisplayFlipbookComponent->SetLooping(loop);\
	itemName##DisplayFlipbookComponent->Play();\
	}

#define CHECK_AND_UPDATE_CURRENT_ID(name,id)\
	if(name > id){name--;}\
	else if(name == id){name=INDEX_NONE;}

	
DEFINE_LOG_CATEGORY_STATIC(SideScrollerCharacter, Log, All);

//////////////////////////////////////////////////////////////////////////
// ARPGCharacter

ARPGCharacterBase::ARPGCharacterBase()
{
	
	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(45.0f);
	GetCapsuleComponent()->SetCapsuleRadius(45.0f);

	GetSprite()->SetRelativeRotation(FRotator(-90,0,90));

	
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Configure character movement
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;

	//setup attack collision
	UpperCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("UpperAttackCollsion"));
	UpperCollision->SetupAttachment(RootComponent);
	UpperCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	UpperCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Overlap);
	UpperCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic,ECollisionResponse::ECR_Overlap);
	
	LowerCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LowerAttackCollision"));
	LowerCollision->SetupAttachment(RootComponent);
	LowerCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	LowerCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Overlap);
	LowerCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic,ECollisionResponse::ECR_Overlap);
		
	
	RightCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightAttackCollsion"));
	RightCollision->SetupAttachment(RootComponent);
	RightCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	RightCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Overlap);
	RightCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic,ECollisionResponse::ECR_Overlap);
	
	LeftCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftAttackCollsion"));
	LeftCollision->SetupAttachment(RootComponent);
	LeftCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	LeftCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Overlap);
	LeftCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic,ECollisionResponse::ECR_Overlap);

	InteractionCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionCollision"));
	InteractionCollision->SetupAttachment(RootComponent);
	InteractionCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	ArmorTopPartDisplayFlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("ArmorTopPartDisplayFlipbook"));
	ArmorTopPartDisplayFlipbookComponent->SetupAttachment(RootComponent);
	ArmorTopPartDisplayFlipbookComponent->SetRelativeScale3D(FVector(3.5f,1,3.5f));

	ArmorMiddlePartDisplayFlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("ArmorMiddlePartDisplayFlipbook"));
	ArmorMiddlePartDisplayFlipbookComponent->SetupAttachment(RootComponent);
	ArmorMiddlePartDisplayFlipbookComponent->SetRelativeScale3D(FVector(3.5f,1,3.5f));

	ArmorBottomPartDisplayFlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("ArmorBottomPartDisplayFlipbook"));
	ArmorBottomPartDisplayFlipbookComponent->SetupAttachment(RootComponent);
	ArmorBottomPartDisplayFlipbookComponent->SetRelativeScale3D(FVector(3.5f,1,3.5f));

	ShieldDisplayFlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("ShieldDisplayFlipbook"));
	ShieldDisplayFlipbookComponent->SetupAttachment(RootComponent);
	ShieldDisplayFlipbookComponent->SetRelativeScale3D(FVector(3.5f,1,3.5f));
	

	// Behave like a traditional 2D platformer character, with a flat bottom instead of a curved capsule bottom
	// Note: This can cause a little floating when going up inclines; you can choose the tradeoff between better
	// behavior on the edge of a ledge versus inclines by setting this to true or false
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;

	// Enable replication on the Sprite component so animations show up when networked
	GetSprite()->SetIsReplicated(true);

	GetSprite()->OnFinishedPlaying.AddDynamic(this,&ARPGCharacterBase::OnFlipbookFinishedPlaying);
	
	bReplicates = true;
}

//////////////////////////////////////////////////////////////////////////
// Animation
#pragma region DirectionalAnimFuncs
UPaperFlipbook* ARPGCharacterBase::GetIdleAnimation()
{
	switch (CurrentDirection)
	{
	case EDirection::ED_Up:
		return IdleAnimationUp;
		break;

	case EDirection::ED_Down:
		return IdleAnimationDown;
		break;

	case EDirection::ED_Right:
		return IdleAnimationRight;
		break;

	case EDirection::ED_Left:
		return IdleAnimationLeft;
		break;
	default:
		return IdleAnimationUp;
	}
}

void ARPGCharacterBase::UpdateCurrentItemIds(int idOfRemoved)
{
	//because it was only on id that was removed we just shift the id of the rest
	CHECK_AND_UPDATE_CURRENT_ID(CurrentWeaponId,idOfRemoved)
	CHECK_AND_UPDATE_CURRENT_ID(TopPartArmorItemId,idOfRemoved)
	CHECK_AND_UPDATE_CURRENT_ID(MiddlePartArmorItemId,idOfRemoved);
	CHECK_AND_UPDATE_CURRENT_ID(BottomPartArmorItemId,idOfRemoved);
	CHECK_AND_UPDATE_CURRENT_ID(ShieldItemId,idOfRemoved);
}

UPaperFlipbook* ARPGCharacterBase::GetRunningAnimation()
{
	switch (CurrentDirection)
	{
	case EDirection::ED_Up:
		return RunningAnimationUp;
		break;

	case EDirection::ED_Down:
		return RunningAnimationDown;
		break;

	case EDirection::ED_Right:
		return RunningAnimationRight;
		break;

	case EDirection::ED_Left:
		return RunningAnimationLeft;
		break;
	default:
		return RunningAnimationUp;
	}
}

UPaperFlipbook* ARPGCharacterBase::GetAttackAnimation()
{	
	switch (CurrentDirection)
	{
	case EDirection::ED_Up:
		return AttackAnimationUp;
		break;

	case EDirection::ED_Down:
		return AttackAnimationDown;
		break;

	case EDirection::ED_Right:
		return AttackAnimationRight;
		break;

	case EDirection::ED_Left:
		return AttackAnimationLeft;
		break;
	default:
		return AttackAnimationUp;
	}
}

UPaperFlipbook* ARPGCharacterBase::GetShieldDrawnAnimation()
{
	switch (CurrentDirection)
	{
	case EDirection::ED_Up:
		return ShieldDrawnAnimationUp;
		break;

	case EDirection::ED_Down:
		return ShieldDrawnAnimationDown;
		break;

	case EDirection::ED_Right:
		return ShieldDrawnAnimationRight;
		break;

	case EDirection::ED_Left:
		return ShieldDrawnAnimationLeft;
		break;
		default:
			return ShieldDrawnAnimationUp;
	}
}

UPaperFlipbook* ARPGCharacterBase::GetDodgeRollAnimation()
{
	switch (CurrentDirection)
	{
	case EDirection::ED_Up:
		return DodgeRollAnimationUp;
		break;

	case EDirection::ED_Down:
		return DodgeRollAnimationDown;
		break;

	case EDirection::ED_Right:
		return DodgeRollAnimationRight;
		break;

	case EDirection::ED_Left:
		return DodgeRollAnimationLeft;
		break;
		default:
			return DodgeRollAnimationUp;
	}
}
#pragma endregion

void ARPGCharacterBase::OnFlipbookFinishedPlaying()
{
	if(bAttacking){bAttacking = false;}
	if (bPlayingDodgeRollAnimation)
	{
		bPlayingDodgeRollAnimation = false;
		OnFinishedDodgeRoll();
	}
	if(bPlayingAnimMontage){bPlayingAnimMontage = false; bPlayingTopPriorityAnimMontage = false;}
	GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Green,"Anim finished: "+GetSprite()->GetFlipbook()->GetName());
}

void ARPGCharacterBase::OnFinishedDodgeRoll_Implementation()
{
	//do stuff when finished, like calling an ability or something
}

bool ARPGCharacterBase::PlayFlipbookAnimation(UPaperFlipbook*Animation,bool bTopPriority,float &length)
{
	length = -1;
	if(Animation != nullptr)
	{
		GetSprite()->SetFlipbook(Animation);
		GetSprite()->SetLooping(false);
		bPlayingAnimMontage = true;
		bPlayingTopPriorityAnimMontage = bTopPriority;
		length = GetSprite()->GetFlipbookLength();
		return true;
	}
	return false;
}

void ARPGCharacterBase::PlayAnimationOnItems(FString animationName,bool looping)
{
	if(animationName != "")
	{
		if(TopPartArmorItemId != -1)
		{
			PlAY_SPECIAL_ANIMATION_ON_ITEM(ArmorTopPart,animationName,false);
		}
		if(MiddlePartArmorItemId != -1)
		{
			PlAY_SPECIAL_ANIMATION_ON_ITEM(ArmorMiddlePart,animationName,false);
		}
		if(BottomPartArmorItemId != -1)
		{
			PlAY_SPECIAL_ANIMATION_ON_ITEM(ArmorBottomPart,animationName,false);
		}
		if(ShieldItemId != -1)
		{
			PlAY_SPECIAL_ANIMATION_ON_ITEM(Shield,animationName,false);
		}
	}
}

void ARPGCharacterBase::StopMovement(float ForHowLong)
{
	if(ForHowLong > 0)
	{
		if(MovementStopTimerHandle.IsValid()){MovementStopTimerHandle.Invalidate();}
		GetWorldTimerManager().SetTimer(MovementStopTimerHandle,this,&ARPGCharacterBase::ResumeMovement,ForHowLong);
		GetCharacterMovement()->MaxWalkSpeed = 0;
	}
}

void ARPGCharacterBase::ResumeMovement()
{
	if(MovementStopTimerHandle.IsValid()){MovementStopTimerHandle.Invalidate();}
	GetCharacterMovement()->MaxWalkSpeed = DefaultMovementSpeed;
}

void ARPGCharacterBase::SetAnimation(UPaperFlipbook* animation, bool Loop)
{
	if(GetSprite()->GetFlipbook() != animation)
	{
		GetSprite()->SetFlipbook(animation);
		GetSprite()->SetLooping(Loop);
		GetSprite()->Play();
	}
}


void ARPGCharacterBase::UpdateAnimation()
{
	if(bUseAnimationSystem)
	{
		const FVector PlayerVelocity = GetVelocity();
		const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();
		if(!bPlayingTopPriorityAnimMontage)
		{
			if(bAttacking && GetAttackAnimation() != nullptr)
			{	
				SetAnimation(GetAttackAnimation(),false);
				if(TopPartArmorItemId != -1)
				{
					PlAY_ANIMATION_ON_ITEM(ArmorTopPart,Attack,false);
				}
				if(MiddlePartArmorItemId != -1)
				{
					PlAY_ANIMATION_ON_ITEM(ArmorMiddlePart,Attack,false);
				}
				if(BottomPartArmorItemId != -1)
				{
					PlAY_ANIMATION_ON_ITEM(ArmorBottomPart,Attack,false);
				}
				if(ShieldItemId != -1)
				{
					PlAY_ANIMATION_ON_ITEM(Shield,Attack,false);
				}
			}
			else if(bPlayingDodgeRollAnimation && GetDodgeRollAnimation() != nullptr)
			{
				SetAnimation(GetDodgeRollAnimation(),false);
				if(TopPartArmorItemId != -1)
				{
					PlAY_ANIMATION_ON_ITEM(ArmorTopPart,DodgeRoll,false);
				}
				if(MiddlePartArmorItemId != -1)
				{
					PlAY_ANIMATION_ON_ITEM(ArmorMiddlePart,DodgeRoll,false);
				}
				if(BottomPartArmorItemId != -1)
				{
					PlAY_ANIMATION_ON_ITEM(ArmorBottomPart,DodgeRoll,false);
				}
				if(ShieldItemId != -1)
				{
					PlAY_ANIMATION_ON_ITEM(Shield,DodgeRoll,false);
				}
			}
			else if(bIsShieldPutUp && GetShieldDrawnAnimation() != nullptr)
			{
				SetAnimation(GetShieldDrawnAnimation(),true);
				if(TopPartArmorItemId != -1)
				{
					PlAY_ANIMATION_ON_ITEM(ArmorTopPart,ShieldDrawn,false);
				}
				if(MiddlePartArmorItemId != -1)
				{
					PlAY_ANIMATION_ON_ITEM(ArmorMiddlePart,ShieldDrawn,false);
				}
				if(BottomPartArmorItemId != -1)
				{
					PlAY_ANIMATION_ON_ITEM(ArmorBottomPart,ShieldDrawn,false);
				}
				if(ShieldItemId != -1)
				{
					PlAY_ANIMATION_ON_ITEM(Shield,ShieldDrawn,false);
				}
			}
			else if(!bPlayingAnimMontage)
			{
				// Are we moving or standing still?
				UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? GetRunningAnimation() : GetIdleAnimation();
				SetAnimation(DesiredAnimation,true);
				if(TopPartArmorItemId != -1)
				{
					PlAY_ANIMATION_ON_ITEM_WITH_SELECTION(ArmorTopPart,Running,Idle,(PlayerSpeedSqr > 0.0f),true);
				}
				if(MiddlePartArmorItemId != -1)
				{
					PlAY_ANIMATION_ON_ITEM_WITH_SELECTION(ArmorMiddlePart,Running,Idle,(PlayerSpeedSqr > 0.0f),true);
				}
				if(BottomPartArmorItemId != -1)
				{
					PlAY_ANIMATION_ON_ITEM_WITH_SELECTION(ArmorBottomPart,Running,Idle,(PlayerSpeedSqr > 0.0f),true);
				}
				if(ShieldItemId != -1)
				{
					PlAY_ANIMATION_ON_ITEM_WITH_SELECTION(Shield,Running,Idle,(PlayerSpeedSqr > 0.0f),true);
				}
			}
		}
	}
}

void ARPGCharacterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	UpdateCharacter();	
}




//////////////////////////////////////////////////////////////////////////
// Input

void ARPGCharacterBase::InteractAction_Implementation()
{
	TArray<AActor*> Interacted;
	InteractionCollision->GetOverlappingActors(Interacted);
	if(Interacted.Num()>0)
	{
		for(int i=0;i<Interacted.Num();i++)
		{
			if (Interacted[i]->Implements<UInteraction>() || (Cast<IInteraction>(Interacted[i]) != nullptr))
			{
				IInteraction::Execute_Interact(Interacted[i],this);
			}
		}
	}
}


void ARPGCharacterBase::AddMoney_Implementation(int amount)
{
	Money+=amount;
	UpdatePlayerInfo();
}

void ARPGCharacterBase::UpdatePlayerInfo_Implementation()
{
}

FItemInfo ARPGCharacterBase::GetItemById(int32 id, bool& hasItem)
{
	hasItem = false;
	if (Items.IsValidIndex(id))
	{
		hasItem = true;
		return Items[id];
	}
	else { return FItemInfo(); }
}

FItemInfo ARPGCharacterBase::GetItemByName(FString devName,bool&hasItem)
{
	hasItem = false;
	if(Items.Num() > 0)
	{
		for(int i=0;i<Items.Num();i++)
		{
			if(Items[i].DevName == devName){hasItem = true; return Items[i];}
		}
	}
	return FItemInfo();
}

int ARPGCharacterBase::GetItemAmountByName(FString devName, bool& hasItem)
{
	hasItem = false;
	if(Items.Num() > 0)
	{
		int Result = 0;
		for (int i = 0; i < Items.Num(); i++)
		{
			if (Items[i].DevName == devName)
			{
				hasItem = true;
				Result += Items[i].CurrentAmount;
			}
		}
		return Result;
	}
	return 0;
}

bool ARPGCharacterBase::RemoveItem(const FString &devName, int amount)
{
	bool hasItem;
	GetItemByName(devName,hasItem);//more compact check
	if(hasItem)
	{
		int count = amount;
		for (int i = 0; i < Items.Num(); i++)
		{
			if(Items[i].DevName == devName)
			{
				if(Items[i].CurrentAmount < count)
				{
					count -= Items[i].CurrentAmount;
					Items.RemoveAt(i);
					UpdateCurrentItemIds(i);
				}
				if(Items[i].CurrentAmount > count)
				{
					Items[i].CurrentAmount -= count;
					return true;//we successfully removed needed amount
				}
				else if(Items[i].CurrentAmount == count)
				{
					Items.RemoveAt(i);
					UpdateCurrentItemIds(i);
					return true;//we successfully removed item
				}
			}
		}
	}
	return false;
}

bool ARPGCharacterBase::PutOnAccessory_Implementation()
{
	if(AccessoryItemId != INDEX_NONE)
	{
		if(Items[AccessoryItemId].SpecialEffect)
		{
			CurrentAccessoryEffect = Cast<ASpecialEffect>(GetWorld()->SpawnActor(Items[AccessoryItemId].SpecialEffect));
			if(CurrentAccessoryEffect)
			{
				CurrentAccessoryEffect->AbilityId = AccessoryItemId;//for abilities that need to wait for player to release the button
				CurrentAccessoryEffect->Damage *= AttackPower;//damage of the ability will increase with player's growing
				CurrentAccessoryEffect->OnAccessoryPutOn(this);
				return true;
			}
		}
	}
	return false;
}

bool ARPGCharacterBase::AddItem_Implementation(FItemInfo item)
{
	int Count = item.CurrentAmount;
	
	if (Items.Num() > 0)
	{
		for (int i = 0; i < Items.Num(); i++)
		{
			if ((Items[i].DevName == item.DevName && Items[i].MaxStackAmount > Items[i].CurrentAmount) && Count > 0)
			{
				while( Items[i].MaxStackAmount > Items[i].CurrentAmount && Count > 0)
				{
					Items[i].CurrentAmount++;
					Count--;
				}
			}
		}
	}
	if(Count > 0)
	{
		item.CurrentAmount = Count;
		Items.Add(item);
	}
	
	return true;
}

bool ARPGCharacterBase::SetCurrentItemById_Implementation(int id, EItemType type)
{
	if(id != INDEX_NONE)
	{
		bool hasItem = false;
		const FItemInfo item = GetItemById(id, hasItem);
		if (hasItem)
		{
			if (item.Type == type)
			{
				switch (type)
				{
				case EItemType::EIT_ArmorTop:
					TopPartArmorItemId = id;
					ArmorTopPartInfo = item;
					return true;
				case EItemType::EIT_ArmorBottom:
					BottomPartArmorItemId = id;
					ArmorBottomPartInfo = item;
					return true;
				case EItemType::EIT_ArmorMiddle:
					MiddlePartArmorItemId = id;
					ArmorMiddlePartInfo = item;
					return true;
				case EItemType::EIT_Weapon:
					CurrentWeaponId = id;
					return true;
				case EItemType::EIT_Shield:
					ShieldItemId = id;
					ShieldInfo = item;
					return true;
				case EItemType::EIT_Accessory:
					if(AccessoryItemId != INDEX_NONE){TakeOffAccessory();}
					AccessoryItemId = id;
					GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,"EItemType::EIT_Accessory");
					return PutOnAccessory();
				default:
					/*nothing else can be equipped*/
					return false;
				}
			}
		}
		return false;
	}
	else
	{
		switch (type)
		{
		case EItemType::EIT_ArmorTop:
			TopPartArmorItemId = id;
			ArmorTopPartInfo = FItemInfo();
			return true;
		case EItemType::EIT_ArmorBottom:
			BottomPartArmorItemId = id;
			ArmorBottomPartInfo = FItemInfo();
			return true;
		case EItemType::EIT_ArmorMiddle:
			MiddlePartArmorItemId = id;
			ArmorMiddlePartInfo = FItemInfo();
			return true;
		case EItemType::EIT_Weapon:
			CurrentWeaponId = id;
			return true;
		case EItemType::EIT_Shield:
			ShieldItemId = id;
			ShieldInfo = FItemInfo();
			return true;
		case EItemType::EIT_Accessory:
			TakeOffAccessory();
			return true;
		default:
			/*nothing else can be equipped*/
			return false;
			return true;
		}
	}
}

void ARPGCharacterBase::Die_Implementation()
{
	if(!bDead)
	{
		bDead = true;
		bUseAnimationSystem = false;
		if(GetController())
		{
			APlayerController *PC = Cast<APlayerController>(GetController());
			if(PC)
			{
				DisableInput(PC);
			}
		}
		OnDied.Broadcast();
	}
}

bool ARPGCharacterBase::TakeOffAccessory_Implementation()
{
	if(AccessoryItemId != INDEX_NONE && CurrentAccessoryEffect)
	{
		CurrentAccessoryEffect->OnAccessoryTakeoff();
		AccessoryItemId = INDEX_NONE;
		return true;
	}
	return false;
}

int ARPGCharacterBase::AddHealth(int AddHealth)
{
	if (Health + AddHealth >= MaxHealth)
	{
		const int Res = (Health + AddHealth)-MaxHealth;
		Health = MaxHealth;
		return Res;
	}
	else
	{
		Health += AddHealth;
		return 0;
	}
}

int ARPGCharacterBase::GetAbilitySlotForAbility(int Id)const
{
	if(Id==Slot1AbilityId){return 0;}
	if(Id==Slot2AbilityId){return 1;}
	if(Id==Slot3AbilityId){return 2;}
	if(Id==Slot4AbilityId){return 3;}
	return -1;
}

bool ARPGCharacterBase::UseAbility(int id)
{
	if(Abilities.IsValidIndex(id))
	{
		if(Abilities[id].AbilityClass && !Abilities[id].bIsCoolingdown && CurrentMagicJuice >= Abilities[id].MannaUsage)
		{
			
			
			if(ASpecialEffect* SE = Cast<ASpecialEffect>(GetWorld()->SpawnActor(Abilities[id].AbilityClass)))
			{
				SE->AbilityId = id;//for abilities that need to wait for player to release the button
				SE->Damage *= AttackPower;//damage of the ability will increase with player's growing
				SE->ApplyEffect(this,GetActorLocation(),GetWorld(),Abilities[id].bApplyToSpawner?this:nullptr);//MUST BE MANUALLY DESTROYED			
			}
			if(Abilities[id].CooldownTime > 0)
			{
				//start cooldown timer
				Abilities[id].bIsCoolingdown = true;
				GetWorldTimerManager().SetTimer(Abilities[id].CooldownTimerHandle,
                                                FTimerDelegate::CreateUObject
                                                (
                                                    this,
                                                    &ARPGCharacterBase::FinishCooldownOnAbility,
                                                    Abilities[id].DevName
                                                ),
                                                Abilities[id].CooldownTime,false);
			}
			
			if(!Abilities[id].bCanMoveWhileUsing)
			{
				StopMovement(Abilities[id].HowLongCanNotMove);
			}
			
			CurrentMagicJuice -= Abilities[id].MannaUsage;
			if(!MagicJuiceRestorationTimerHandle.IsValid())
			{
				GetWorldTimerManager().SetTimer(MagicJuiceRestorationTimerHandle,this,&ARPGCharacterBase::RestoreMagicJuice,MagicJuiceRestorationTime,true);
			}
			
			UpdatePlayerInfo();
			
			return true;
		}	
	}
	return false;
}

bool ARPGCharacterBase::UseAbilityByName(FString Name)
{
	if (Abilities.Num() > 0)
	{
		for (int i = 0; i < Abilities.Num(); i++)
		{
			if(Abilities[i].DevName == Name)
			{
				return UseAbility(i);
			}
		}
	}
	return false;
}

bool ARPGCharacterBase::AddAbility(FAbilityInfo Ability)
{
	if(Abilities.Num() > 0)
	{
		for (int i = 0; i < Abilities.Num(); i++)
		{
			if (Abilities[i].DevName == Ability.DevName) { return false; }
		}
	}
	Abilities.Add(Ability);
	return true;
}

FAbilityInfo ARPGCharacterBase::GetAbilityInfo(int id, bool& has)const
{
	has = false;
	if(Abilities.IsValidIndex(id))
	{
		has = true;
		return Abilities[id];
	}
	return FAbilityInfo();
}

FAbilityInfo ARPGCharacterBase::GetAbilityInfoByName(FString name, bool& has)const
{
	has = false;
	if (Abilities.Num() > 0)
	{
		for (int i = 0; i < Abilities.Num(); i++)
		{
			if(Abilities[i].DevName == name){has = true; return Abilities[i];}
		}
	}
	
	return FAbilityInfo();
}

int ARPGCharacterBase::GetAbilityId(FString name)
{
	if (Abilities.Num() > 0)
	{
		for (int i = 0; i < Abilities.Num(); i++)
		{
			if(Abilities[i].DevName==name){return i;}
		}
	}
	return -1;
}

void ARPGCharacterBase::FinishCooldownOnAbility(FString devName)
{
	int id = GetAbilityId(devName);
	if(id !=-1)
	{
		Abilities[id].bIsCoolingdown = false;
		Abilities[id].CooldownTimerHandle.Invalidate();
	}
}

void ARPGCharacterBase::RestoreMagicJuice()
{
	CurrentMagicJuice += MagicJuiceRestorationAmount;
	if(CurrentMagicJuice >= MaxMagicJuice)
	{
		GetWorldTimerManager().ClearTimer(MagicJuiceRestorationTimerHandle);
	}
	UpdatePlayerInfo();
}

int ARPGCharacterBase::AddMagicJuice(int Amount)
{
	if(CurrentMagicJuice + Amount > MaxMagicJuice)
	{
		const int Res = CurrentMagicJuice + Amount - MaxMagicJuice;
		CurrentMagicJuice = MaxMagicJuice;
		return Res;
	}
	else
	{
		CurrentMagicJuice += Amount;
		return 0;
	}
}

FItemInfo ARPGCharacterBase::GetCurrentItemForType(bool& has, EItemType type)
{
	has = false;
	switch (type)
	{
		case EItemType::EIT_Weapon:
			if(Items.IsValidIndex(CurrentWeaponId)){has = true; return Items[CurrentWeaponId];}break;
		case EItemType::EIT_ArmorTop:
			if(Items.IsValidIndex(TopPartArmorItemId)){has = true; return Items[TopPartArmorItemId];}break;
		case EItemType::EIT_ArmorMiddle:
			if(Items.IsValidIndex(BottomPartArmorItemId)){has = true; return Items[BottomPartArmorItemId];}break;
		case EItemType::EIT_ArmorBottom:
			if(Items.IsValidIndex(BottomPartArmorItemId)){has = true; return Items[BottomPartArmorItemId];}break;
		case EItemType::EIT_Shield:
			if(Items.IsValidIndex(ShieldItemId)){has = true; return Items[ShieldItemId];}break;
		case EItemType::EIT_Accessory:
			if(Items.IsValidIndex(AccessoryItemId)){has = true; return Items[AccessoryItemId];}break;
		default:
			return FItemInfo();
	}
	return FItemInfo();
}


void ARPGCharacterBase::Roll(EDirection Direction,float forceMultiplier)
{
	if(bCanDodgeRoll)
	{
		FVector Velocity;
		switch (Direction)
		{
		case EDirection::ED_Up:
			Velocity = FVector(0,-1,0);
			break;
		case EDirection::ED_Down:
			Velocity = FVector(0,1,0);
			break;
		case EDirection::ED_Left:
			Velocity = FVector(-1,0,0);
			break;
		case EDirection::ED_Right:
			Velocity = FVector(1,0,0);
			break;
		}
		if(GetDodgeRollAnimation())
		{
			bPlayingDodgeRollAnimation = true;
		}
		LaunchCharacter(Velocity*DodgeRollSpeed*forceMultiplier,false,false);
		bCanDodgeRoll = false;
		GetWorldTimerManager().SetTimer(DodgeRollCooldownTimerHandle,this,&ARPGCharacterBase::EndDodgeRollCooldown,DodgeRollCooldownTime);
	}
}

bool ARPGCharacterBase::PutUpShield()
{
	if(ShieldItemId != -1)
	{
		bIsShieldPutUp = true;
		if(UCharacterMovementComponent * MovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent()))
		{
			bool has;
			MovementComponent->MaxWalkSpeed = DefaultMovementSpeed*GetItemById(ShieldItemId,has).MovementSpeedDecreaseMultiplier;
			return true;
		}		
	}
	return false;
}

bool ARPGCharacterBase::PutDownShield()
{
	if(ShieldItemId != -1)
	{
		bIsShieldPutUp = false;
		if(UCharacterMovementComponent * MovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent()))
		{
			MovementComponent->MaxWalkSpeed = DefaultMovementSpeed;
			return true;
		}
	}
	return false;
}

void ARPGCharacterBase::EndDodgeRollCooldown()
{
	bCanDodgeRoll = true;
}

void ARPGCharacterBase::ReactToDamage_Implementation()
{
	UpdatePlayerInfo();
	//Play camera shakes, damage anims, etc. here
}

void ARPGCharacterBase::DoPassiveAbilitiesCheck()
{
	if (Abilities.Num() > 0)
	{
		for (int i = 0; i < Abilities.Num(); i++)
		{
			if (Abilities[i].bIsPassive && Abilities[i].CheckObjectClass)
			{
				if (Abilities[i].CheckObjectClass.GetDefaultObject()->DoCheck(this))
				{
					UseAbility(i);
				}
			}
		}
	}
}

int ARPGCharacterBase::DealDamage_Implementation(int Damage,AActor*DamageDealer, TSubclassOf<ASpecialEffect> SpecialEffect)
{
	//this character can not die and as such doesn't receive damage
	if (!bCanDie) { return 0; }
	int TotalArmor = Defense;
	bool hasArmorPiece = false;
	FItemInfo item = GetCurrentItemForType(hasArmorPiece,EItemType::EIT_ArmorBottom);
	if (hasArmorPiece)
	{
		TotalArmor += item.Defence;
	}
	item = GetCurrentItemForType(hasArmorPiece,EItemType::EIT_ArmorTop);
	if (hasArmorPiece)
	{
		TotalArmor += item.Defence;
	}
	item = GetCurrentItemForType(hasArmorPiece,EItemType::EIT_ArmorMiddle);
	if (hasArmorPiece)
	{
		TotalArmor += item.Defence;
	}
	item = GetCurrentItemForType(hasArmorPiece,EItemType::EIT_Shield);
	if (hasArmorPiece)
	{
		TotalArmor += bIsShieldPutUp ? item.Defence : 0;
	}

	Health -= ((Damage - TotalArmor) >= 0) ? (Damage - TotalArmor) : 0;
	if(Health <= 0){Health = 0; Killer = Cast<ARPGCharacterBase>(DamageDealer);Die();}
	else if(SpecialEffect)
	{
		if(SpecialEffect.GetDefaultObject()->bDoesEffectLasts)
		{
			ASpecialEffect* SE = Cast<ASpecialEffect>(GetWorld()->SpawnActor(SpecialEffect));
			if(SE)
			{
				SE->ApplyEffect(DamageDealer,GetActorLocation(),GetWorld(),this);//this will get destroyed ONLY after time passes
			}
		}
		else
		{
			SpecialEffect.GetDefaultObject()->ApplyEffect(DamageDealer,GetActorLocation(),GetWorld(),this);//this will get destroyed after function executes
		}
	}
	ReactToDamage();
	return Damage;
}


void ARPGCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	if(GetController())
	{
		APlayerController*PC  = Cast<APlayerController>(GetController());
		if(PC)
		{
			PC->bShowMouseCursor = true;
			PC->SetInputMode(FInputModeGameAndUI());
		}
	}
	GetWorldTimerManager().SetTimer(PassiveAbilityCheckTimerHandle, this, &ARPGCharacterBase::DoPassiveAbilitiesCheck,
	                                0.5f, true);
}

void ARPGCharacterBase::NotifyAboutEffectStart_Implementation(ASpecialEffect* Effect)
{
	CurrentlyAppliedEffects.Add(Effect);//we add effect to remember about it
}

void ARPGCharacterBase::CleanAppliedEffects()
{
	//Using foreach because using usual for will result in wrong indexation after removal
	for (ASpecialEffect* effect : CurrentlyAppliedEffects)
	{
		if(effect == nullptr){CurrentlyAppliedEffects.Remove(effect);}
	}
}

void ARPGCharacterBase::UpdateCharacter()
{
	

	// Now setup the rotation of the controller based on the direction we are travelling
	const FVector PlayerVelocity = GetVelocity();
	
	float HorizontalDirection =  FMath::RoundToInt(PlayerVelocity.X/DefaultMovementSpeed);
	
	float VerticalDirection = FMath::RoundToInt(PlayerVelocity.Y/DefaultMovementSpeed);
	
	// Set the rotation so that the character faces his direction of travel.
	if (Controller != nullptr)
	{
		if(HorizontalDirection < 0.f  && VerticalDirection == 0.f)
		{
			CurrentDirection = EDirection::ED_Left;
		}
		if(HorizontalDirection > 0.f && VerticalDirection == 0.f)
		{
			CurrentDirection = EDirection::ED_Right;
		}
		if(VerticalDirection < 0.f && HorizontalDirection == 0.f)
		{
			CurrentDirection = EDirection::ED_Up;
		}
		if(VerticalDirection > 0.f && HorizontalDirection == 0.f)
		{
			CurrentDirection = EDirection::ED_Down;
		}
	}

	// Update animation to match the motion
	UpdateAnimation();
}

void ARPGCharacterBase::ResetCombo_Implementation()
{
	if(ComboAttackResetTimerHandle.IsValid()){ComboAttackResetTimerHandle.Invalidate();}
	ComboAttackCurrentCount = 0;
}

void ARPGCharacterBase::ApplyComboBonus_Implementation()
{
	if(ComboAttackResetTimerHandle.IsValid()){ComboAttackResetTimerHandle.Invalidate();}
	ComboAttackCurrentCount = 0;
}

bool ARPGCharacterBase::CanAttack()
{
	return (!bPlayingAnimMontage && !bIsShieldPutUp && !bAttacking);
}


void ARPGCharacterBase::Attack_Implementation(float damageMultiplier)
{
	if(CanAttack())
	{
		if(GetAttackAnimation())
		{
			bAttacking = true;
		}
		//4 direction hit system -> based on where player is looking
		TArray<AActor*> AttackedActors;
		switch (CurrentDirection)
		{
		case EDirection::ED_Up:
			UpperCollision->GetOverlappingActors(AttackedActors);
			break;
		case EDirection::ED_Down:
			LowerCollision->GetOverlappingActors(AttackedActors);
			break;
		case EDirection::ED_Right:
			RightCollision->GetOverlappingActors(AttackedActors);
			break;
		case EDirection::ED_Left:
			LeftCollision->GetOverlappingActors(AttackedActors);
			break;
		}
		bool hasItem;
		const FItemInfo Item = GetItemById(CurrentWeaponId,hasItem);
		if(hasItem)
		{
			if(Item.Type == EItemType::EIT_Weapon)
			{
				if(Item.SpecialEffect != nullptr)
				{
					if(!Item.SpecialEffect.GetDefaultObject()->bLocal)
					{
						ASpecialEffect* SE = Cast<ASpecialEffect>(GetWorld()->SpawnActor(Item.SpecialEffect));
						if(SE)
						{					
							SE->ApplyEffect(this,GetActorLocation(),GetWorld(),nullptr);//MUST BE MANUALLY DESTROYED
						}			
						//Item.SpecialEffect.GetDefaultObject()->ApplyEffect(this,GetActorLocation(),GetWorld(),nullptr);
					}
				}
			}
		

			bool AnyoneWasHit = false;
			if(AttackedActors.Num() > 0)
			{
				for (int i = 0; i < AttackedActors.Num(); i++)
				{				
					if (AttackedActors[i] != this && (AttackedActors[i]->Implements<UInteraction>() || (Cast<IInteraction>(GetOwner()) != nullptr)))
					{
						AnyoneWasHit = true;
						IInteraction::Execute_DealDamage(AttackedActors[i], Item.Attack*AttackPower*damageMultiplier,this, Item.SpecialEffect);
						//next section is bad code. Do not do things this way
						if(CurrentlyAppliedEffects.Num() > 0)
						{
							for (int ind = 0; ind < CurrentlyAppliedEffects.Num(); ind++)
							{
								if(CurrentlyAppliedEffects[ind])//check if it's valid
								{
									//we don't need to check for it if doesn't apply any enhancement(or has none)
									if(CurrentlyAppliedEffects[ind]->bHasWeaponEnhancement && CurrentlyAppliedEffects[ind]->WeaponEnhancementEffect != nullptr )
									{
										//create this variables instead of constantly calling functions
										auto Eff = CurrentlyAppliedEffects[ind];
										const EEffectType WeaponEffectType = (Item.SpecialEffect != nullptr)
                                                                                 ? Item.SpecialEffect.GetDefaultObject()->Type
                                                                                 : EEffectType::EET_None;
										bool success = true;
										if(Eff->IncompatibleTypes.Num() > 0 && WeaponEffectType != EEffectType::EET_None)
										{
											//loop thru all of the incompatible types to see if it can be used(to avoid applying fire effect to weapon that freezes enemies)
											for(int a = 0;a < Eff->IncompatibleTypes.Num();a++)
											{
												if(WeaponEffectType == Eff->IncompatibleTypes[a] && WeaponEffectType != EEffectType::EET_None)
												{
													//can not do anything
													success = false;
													break;//end loop of checking
												}
											}
										}
										if(success)
										{
											//set damage to zero because we already applied damage
											IInteraction::Execute_DealDamage(AttackedActors[i], 0,this, Eff->WeaponEnhancementEffect);
										}
									}
								}
							}
						}

						//do combo counting
						ComboAttackCurrentCount++;
						if(ComboAttackCurrentCount >= ComboAttackNeededCount)
						{
							ApplyComboBonus();
						}
						else if(ComboAttackCurrentCount == 1)
						{
							//if it's the fist time we attack since last combo reset we will create new reset timer
							GetWorldTimerManager().SetTimer(ComboAttackResetTimerHandle, this,
							                                &ARPGCharacterBase::ResetCombo, ComboAttackResetTime,
							                                false);
						}
					}
				}
			}
			if(!AnyoneWasHit)
			{
				ResetCombo();
			}
		}
	}
}

