// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "RPGCore/Public/Ability/AbilityInfo.h"
#include "Components/BoxComponent.h"
#include "RPGCore/Public/Item/ItemInfo.h"
#include "RPGCore/Public/Quest/QuestInfo.h"


#include "RPGCharacterBase.generated.h"


class UTextRenderComponent;

UENUM(BlueprintType)
enum class EDirection:uint8
{
	ED_Up UMETA(DisplayName = "Up"),
	ED_Down UMETA(DisplayName = "Down"),
	ED_Right UMETA(DisplayName = "Right"),
	ED_Left UMETA(DisplayName = "Left")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestFinished,FQuestInfo,Quest);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelUped,int,CurrentLevel);
/**
 * This class is the default character for RPG, and it is responsible for all
 * physical interaction between the player and the world.
 *
 * The capsule component (inherited from ACharacter) handles collision with the world
 * The CharacterMovementComponent (inherited from ACharacter) handles movement of the collision capsule
 * The Sprite component (inherited from APaperCharacter) handles the visuals
 */
UCLASS(config=Game)
class ARPGCharacterBase : public APaperCharacter
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
	bool AddQuestToQuestArray(FQuestInfo quest);

	/*Use this function if you want to give player a quest
	 *Returns false if player already has this quest
	 * 
	 */
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=Quest)
	bool AddQuest(FQuestInfo quest);

	/*
	 *Returns false if quest was already completed
	 *Do not use this function if you want to finish quest, use @MakeProgress instead
	 */
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=Quest)
	bool CompleteQuest(const FString &devName,int questId);

	/*Used by dialog system to mark quests as rewarded and avoid giving reward more than once
	 * Do not use manually
	 */
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=Quest)
	void MarkQuestAsRewarded(const FString &devName);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=Quest)
	void MakeProgress(const FString &devName,int amount);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=Quest)
	void ChangeQuestInfo(FQuestInfo newQuestInfo);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=Display)
	void UpdateQuestDisplayInfo();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=Display)
	void UpdatePlayerInfo();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=Exp)
	void AddExp(int amount);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=Exp)
	void LevelUp();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=Money)
    void AddMoney(int amount);

	UFUNCTION(BlueprintCallable,Category= Items)
	virtual FItemInfo GetItemById(int32 id,bool&hasItem);

	/*Add new item to player's inventory. Never returns false(added for future)*/
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=Items)
	bool AddItem(FItemInfo item);

	/*Set current weapon,armor to be one of the items from inventory. Use this to equip weapons etc.*/
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=Items)
	bool SetCurrentItemById(int id,EItemType type);

	/*Returns false if ability can not be used*/
	UFUNCTION(BlueprintCallable)
	virtual bool UseAbility(int id);
	
	/*Returns false if ability can not be used*/
	UFUNCTION(BlueprintCallable)
    virtual bool UseAbilityByName(FString Name);

	UFUNCTION(BlueprintCallable)
	virtual FAbilityInfo GetAbilityInfo(int id,bool&has);

	UFUNCTION(BlueprintCallable)
    virtual FAbilityInfo GetAbilityInfoByName(FString name,bool&has);

	/*This return COPY of quest info, changing it will NOT affect original info. See: @ChangeQuestInfo for that
	 *
	 */
	UFUNCTION(BlueprintPure,Category=Quest)
	FQuestInfo GetQuestInfo(FString devName,bool&hasQuest);

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

	UPROPERTY(BlueprintAssignable)
	FOnLevelUped OnLevelUped;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= Direction,SaveGame)
	EDirection CurrentDirection;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= Info,SaveGame)
	int Experience = 0;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= Info,SaveGame)
	int Level = 0;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= Info,SaveGame)
	int Money = 0;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= Skill,SaveGame)
	int SkillPoints = 0;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= Level,SaveGame)
	int NeededExperienceMultiplier = 100;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= Stats,SaveGame)
	int AttackPower = 1;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= Stats,SaveGame)
	int Defense = 1;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= Sound,SaveGame)
	USoundBase *AttackSound  = nullptr;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Quest,SaveGame)
	TArray<FQuestInfo> Quests;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Abilities,SaveGame)
	TArray<FAbilityInfo>Abilities;

	/*Every item player has*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Items,SaveGame)
	TArray<FItemInfo>Items;

	/*
	 * To avoid issues with structs(the ones related to creating copies instead of working with originals) id will be used
	 * To get item Use @GetItemById
	 */
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Items_Weapon,SaveGame)
	int32 CurrentWeaponId = -1;

	/*
	* To avoid issues with structs(the ones related to creating copies instead of working with originals) id will be used
	* To get item Use @GetItemById
	*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Items_Armor,SaveGame)
	int32 TopPartArmorItemId = -1;

	/*
	* To avoid issues with structs(the ones related to creating copies instead of working with originals) id will be used
	* To get item Use @GetItemById
	*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Items_Armor,SaveGame)
	int32 MiddlePartArmorItemId = -1;

	/*
	* To avoid issues with structs(the ones related to creating copies instead of working with originals) id will be used
	* To get item Use @GetItemById
	*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Items_Armor,SaveGame)
	int32 BottomPartArmorItemId = -1;
	
	ARPGCharacterBase();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};
