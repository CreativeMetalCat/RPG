// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPGCharacter.h"

#include "InteractionInterface.h"
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

ARPGCharacter::ARPGCharacter()
{
	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(45.0f);
	GetCapsuleComponent()->SetCapsuleRadius(45.0f);

	GetSprite()->SetRelativeRotation(FRotator(-90,0,90));

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
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
	bReplicates = true;
}

//////////////////////////////////////////////////////////////////////////
// Animation

void ARPGCharacter::UpdateAnimation()
{
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

	// Are we moving or standing still?
	UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? RunningAnimation : IdleAnimation;
	if( GetSprite()->GetFlipbook() != DesiredAnimation 	)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
	}
}

void ARPGCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	UpdateCharacter();	
}


//////////////////////////////////////////////////////////////////////////
// Input

void ARPGCharacter::Interact_Implementation()
{
	TArray<AActor*> Interacted;
	InteractionCollision->GetOverlappingActors(Interacted);
	if(Interacted.Num()>0)
	{
		for(int i=0;i<Interacted.Num();i++)
		{
			if (Interacted[i]->Implements<UInteractionInterface>() || (Cast<IInteractionInterface>(Interacted[i]) != nullptr))
			{
				IInteractionInterface::Execute_Interact(Interacted[i],this);
			}
		}
	}
}

bool ARPGCharacter::AddQuest_Implementation(FQuest quest)
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


bool ARPGCharacter::AddQuestToQuestArray(FQuest quest)
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

bool ARPGCharacter::CompleteQuest_Implementation(const FString& devName,int questId)
{
	if(Quests[questId].bCompleted){return false;}
	
	Quests[questId].bCompleted = true;
	OnQuestFinished.Broadcast(Quests[questId]);
	return true;
}

void ARPGCharacter::MakeProgress_Implementation(const FString& devName, int amount)
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
					return;
				}
				else
				{
					return;
				}
			}
		}
	}
}

void ARPGCharacter::ChangeQuestInfo_Implementation(FQuest newQuestInfo)
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

void ARPGCharacter::UpdateQuestDisplayInfo_Implementation()
{
}

FQuest ARPGCharacter::GetQuestInfo(FString devName,bool&hasQuest)
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
	return FQuest();
}

bool ARPGCharacter::HasQuest(const FString& devName)
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

void ARPGCharacter::BeginPlay()
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

void ARPGCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &ARPGCharacter::Attack);

	PlayerInputComponent->BindAction("Interact", IE_Released, this, &ARPGCharacter::Interact);
	
	PlayerInputComponent->BindAxis("MoveRight", this, &ARPGCharacter::MoveRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &ARPGCharacter::MoveUp);
}

void ARPGCharacter::MoveRight(float Value)
{
	/*UpdateChar();*/
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		AddMovementInput(FVector(1,0,0), Value);
	}
}

void ARPGCharacter::MoveUp(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// add movement in that direction
		AddMovementInput(FVector(0,1,0), Value);
	}
}

void ARPGCharacter::UpdateCharacter()
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
		else if(HorizontalDirection> 0.f)
		{
			CurrentDirection = EDirection::ED_Right;
		}
		else if(VerticalDirection < 0.f)
		{
			CurrentDirection = EDirection::ED_Up;
		}
		else if(VerticalDirection > 0.f)
		{
			CurrentDirection = EDirection::ED_Down;
		}
	}
}


void ARPGCharacter::Attack_Implementation()
{
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

	if(AttackedActors.Num()>0)
	{
		for(int i=0;i<AttackedActors.Num();i++)
		{
			UGameplayStatics::ApplyDamage
			(
				AttackedActors[i],
				10.f/*will be based on weapon*/,
				GetController(),
				this,
				UDamageType::StaticClass()/*will be based on weapon*/
			);
		}
	}
}
