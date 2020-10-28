// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"


#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyBase::AEnemyBase()
{

    GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;

    // Set the size of our collision capsule.
    GetCapsuleComponent()->SetCapsuleHalfHeight(45.0f);
    GetCapsuleComponent()->SetCapsuleRadius(45.0f);

    GetSprite()->SetRelativeRotation(FRotator(-90,0,90));
    
    // Enable replication on the Sprite component so animations show up when networked
    GetSprite()->SetIsReplicated(true);
    bReplicates = true;
}
