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

void ARPGCharacterBase::OnFlipbookFinishedPlaying()
{
	if(bAttacking){bAttacking = false;}
	if(bPlayingAnimMontage){bPlayingAnimMontage = false;}
	GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Green,"Anim finished");
}

bool ARPGCharacterBase::PlayFlipbookAnimation(UPaperFlipbook*Animation,float &length)
{
	length = -1;
	if(Animation != nullptr)
	{
		GetSprite()->SetFlipbook(Animation);
		GetSprite()->SetLooping(false);
		bPlayingAnimMontage = true;
		length = GetSprite()->GetFlipbookLength();
		return true;
	}
	return false;
}


void ARPGCharacterBase::UpdateAnimation()
{
	if(bUseAnimationSystem)
	{
		const FVector PlayerVelocity = GetVelocity();
		const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

		if(bAttacking && AttackAnimation != nullptr)
		{	
			if(GetSprite()->GetFlipbook() != AttackAnimation)
			{
				GetSprite()->SetFlipbook(AttackAnimation);
				GetSprite()->SetLooping(false);
			}
		}
		else if(!bPlayingAnimMontage)
		{
			// Are we moving or standing still?
			UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? RunningAnimation : IdleAnimation;
			if( GetSprite()->GetFlipbook() != DesiredAnimation 	)
			{
				GetSprite()->SetFlipbook(DesiredAnimation);

				if(!GetSprite()->IsLooping())
				{
					GetSprite()->SetLooping(true);
					GetSprite()->Play();
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

bool ARPGCharacterBase::RemoveItem(const FString &devName, int amount)
{
	bool hasItem;
	GetItemByName(devName,hasItem);//more compact check
	if(hasItem)
	{
		for (int i = 0; i < Items.Num(); i++)
		{
			if(Items[i].DevName == devName)
			{
				if(Items[i].CurrentAmount >= amount)
				{
					Items[i].CurrentAmount -= amount;
					return true;//we successfully removed needed amount
				}
				else if(Items[i].CurrentAmount == amount)
				{
					Items.RemoveAt(i);
					return true;//we successfully removed item
				}
			}
		}
	}
	return false;
}

bool ARPGCharacterBase::AddItem_Implementation(FItemInfo item)
{
	if(Items.Num() > 0)
	{
		for(int i =0 ;i<Items.Num();i++)
		{
			if(Items[i].DevName == item.DevName && Items[i].MaxStackAmount > Items[i].CurrentAmount)
			{
				Items[i].CurrentAmount++;
			}
		}
	}
	Items.Add(item);
	
	return true;
}

bool ARPGCharacterBase::SetCurrentItemById_Implementation(int id, EItemType type)
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
				return true;
			case EItemType::EIT_ArmorBottom:
				BottomPartArmorItemId = id;
				return true;
			case EItemType::EIT_ArmorMiddle:
				MiddlePartArmorItemId = id;
				return true;
			case EItemType::EIT_Weapon:
				CurrentWeaponId = id;
				return true;
			default:
				/*nothing else can be equipped*/
				return false;
			}
		}
	}
	return false;
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

bool ARPGCharacterBase::UseAbility(int id)
{
	if(Abilities.IsValidIndex(id))
	{
		if(Abilities[id].AbilityClass && !Abilities[id].bIsCoolingdown)
		{
			Abilities[id].bIsCoolingdown = true;
			ASpecialEffect* SE = Cast<ASpecialEffect>(GetWorld()->SpawnActor(Abilities[id].AbilityClass));
			if(SE)
			{					
				SE->ApplyEffect(this,GetActorLocation(),GetWorld(),nullptr);//MUST BE MANUALLY DESTROYED
			}
			
			GetWorldTimerManager().SetTimer(Abilities[id].CooldownTimerHandle,
			                                FTimerDelegate::CreateUObject
			                                (
				                                this,
				                                &ARPGCharacterBase::FinishCooldownOnAbility,
				                                Abilities[id].DevName
				                            ),
				                            Abilities[id].CooldownTime,false);
			
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

FItemInfo ARPGCharacterBase::GetCurrentWeapon(bool& has)
{
	has = false;
	if(Items.IsValidIndex(CurrentWeaponId)){has = true; return Items[CurrentWeaponId];}
	return FItemInfo();
}

FItemInfo ARPGCharacterBase::GetCurrentTopPartArmor(bool& has)
{
	has = false;
	if(Items.IsValidIndex(TopPartArmorItemId)){has = true; return Items[TopPartArmorItemId];}
	return FItemInfo();
}

FItemInfo ARPGCharacterBase::GetCurrentBottomPartArmor(bool& has)
{
	has = false;
	if(Items.IsValidIndex(BottomPartArmorItemId)){has = true; return Items[BottomPartArmorItemId];}
	return FItemInfo();
}

FItemInfo ARPGCharacterBase::GetCurrentMiddleArmor(bool& has)
{
	has = false;
	if(Items.IsValidIndex(BottomPartArmorItemId)){has = true; return Items[BottomPartArmorItemId];}
	return FItemInfo();
}

int ARPGCharacterBase::DealDamage_Implementation(int Damage,AActor*DamageDealer, TSubclassOf<ASpecialEffect> SpecialEffect)
{
	//TODO: Add damage reduction based on stats and whether player holds shield or not
	int TotalArmor = Defense;
	bool hasArmorPiece = false;
	FItemInfo item = GetCurrentBottomPartArmor(hasArmorPiece);
	if(hasArmorPiece)
	{
		TotalArmor += item.Defence;
	}
	GetCurrentTopPartArmor(hasArmorPiece);
	if(hasArmorPiece)
	{
		TotalArmor += item.Defence;
	}
	GetCurrentMiddleArmor(hasArmorPiece);
	if(hasArmorPiece)
	{
		TotalArmor += item.Defence;
	}
	Health-=((Damage - TotalArmor) >=0 )?(Damage - TotalArmor): 0;
	if(Health <= 0){Health = 0; Die();}
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
}




void ARPGCharacterBase::UpdateCharacter()
{
	// Update animation to match the motion
	UpdateAnimation();

	// Now setup the rotation of the controller based on the direction we are travelling
	const FVector PlayerVelocity = GetVelocity();
	
	float HorizontalDirection = PlayerVelocity.X;
	
	float VerticalDirection = PlayerVelocity.Y;
	
	// Set the rotation so that the character faces his direction of travel.
	if (Controller != nullptr)
	{
		if(HorizontalDirection < 0.f)
		{
			CurrentDirection = EDirection::ED_Left;
		}
		if(HorizontalDirection> 0.f)
		{
			CurrentDirection = EDirection::ED_Right;
		}
		if(VerticalDirection < 0.f)
		{
			CurrentDirection = EDirection::ED_Up;
		}
		if(VerticalDirection > 0.f)
		{
			CurrentDirection = EDirection::ED_Down;
		}
	}
}


void ARPGCharacterBase::Attack_Implementation()
{

	if(AttackAnimation)
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
	}

	if(AttackedActors.Num() > 0)
	{
		for(int i=0;i<AttackedActors.Num();i++)
		{
			if (AttackedActors[i] != this && (AttackedActors[i]->Implements<UInteraction>() || (Cast<IInteraction>(GetOwner()) != nullptr)))
			{
				IInteraction::Execute_DealDamage(AttackedActors[i], Item.Attack+AttackPower,this, Item.SpecialEffect);
			}
			
			//TODO: Remove this old damage code(only after new one is tested well enough)
			/*UGameplayStatics::ApplyDamage
			(
				AttackedActors[i],
				10.f,
				GetController(),
				this,
				hasItem?Item.DamageType:UDamageType::StaticClass()
			);*/
		}
	}
}

