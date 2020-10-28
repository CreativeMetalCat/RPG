// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "Components/BoxComponent.h"
#include "Quest/Quest.h"


#include "RPGCharacter.generated.h"


class UTextRenderComponent;

UENUM(BlueprintType)
enum class EDirection:uint8
{
	ED_Up UMETA(DisplayName = "Up"),
	ED_Down UMETA(DisplayName = "Down"),
	ED_Right UMETA(DisplayName = "Right"),
	ED_Left UMETA(DisplayName = "Left")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestFinished,FQuest,Quest);
/**
 * This class is the default character for RPG, and it is responsible for all
 * physical interaction between the player and the world.
 *
 * The capsule component (inherited from ACharacter) handles collision with the world
 * The CharacterMovementComponent (inherited from ACharacter) handles movement of the collision capsule
 * The Sprite component (inherited from APaperCharacter) handles the visuals
 */
UCLASS(config=Game)
class ARPGCharacter : public APaperCharacter
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess="true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UTextRenderComponent* TextComponent;
	virtual void Tick(float DeltaSeconds) override;
protected:
	// The animation to play while running around
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
	class UPaperFlipbook* RunningAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Interaction)
	UBoxComponent *InteractionCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attack)
	UBoxComponent *UpperCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attack)
	UBoxComponent *LowerCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attack)
	UBoxComponent *LeftCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attack)
	UBoxComponent *RightCollision;

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* IdleAnimation;

	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation();

	/** Called for side to side input */
	void MoveRight(float Value);

	void MoveUp(float Value);

	void UpdateCharacter();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=Attack)
	void Attack();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=Interaction)
	void Interact();

	/*This function is NOT for gameplay purposes, but for development*/
	UFUNCTION(BlueprintCallable,Category=Dev)
	bool AddQuestToQuestArray(FQuest quest);

	/*Use this function if you want to give player a quest
	 *Returns false if player already has this quest
	 * 
	 */
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=Quest)
	bool AddQuest(FQuest quest);

	/*
	 *Returns false if quest was already completed
	 *Do not use this function if you want to finish quest, use @MakeProgress instead
	 */
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=Quest)
	bool CompleteQuest(const FString &devName,int questId);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=Quest)
	void MakeProgress(const FString &devName,int amount);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=Quest)
	void ChangeQuestInfo(FQuest newQuestInfo);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=Display)
	void UpdateQuestDisplayInfo();

	/*This return COPY of quest info, changing it will NOT affect original info. See: @ChangeQuestInfo for that
	 *
	 */
	UFUNCTION(BlueprintPure,Category=Quest)
	FQuest GetQuestInfo(FString devName,bool&hasQuest);

	/*Returns true if player has this quest, EVEN if it's completed*/
	UFUNCTION(BlueprintPure,Category=Quest)
	bool HasQuest(const FString &devName);

	virtual void BeginPlay() override;


	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	UPROPERTY(BlueprintAssignable)
	FOnQuestFinished OnQuestFinished;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= Direction)
	EDirection CurrentDirection;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= Exp)
	int Experience = 0;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= Exp)
	int Level = 0;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= Sound)
	USoundBase *AttackSound  = nullptr;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Quest)
	TArray<FQuest> Quests;
	
	ARPGCharacter();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};
