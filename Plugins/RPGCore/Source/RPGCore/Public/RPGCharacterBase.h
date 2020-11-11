// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Interaction.h"
#include "PaperCharacter.h"
#include "RPGCore/Public/Ability/AbilityInfo.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbook.h"
#include "RPGCore/Public/Item/ItemInfo.h"
#include "RPGCore/Public/Quest/QuestInfo.h"


#include "RPGCharacterBase.generated.h"


class UTextRenderComponent;

UENUM(BlueprintType)
enum class EAnimationPlayResult: uint8
{
	EAPR_Success UMETA(DisplayName = "Success"),
	EAPR_Fail UMETA(DisplayName = "Fail")
};

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

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDied);
/**
 * This class is the default character for RPG, and it is responsible for all
 * physical interaction between the player and the world.
 *
 * The capsule component (inherited from ACharacter) handles collision with the world
 * The CharacterMovementComponent (inherited from ACharacter) handles movement of the collision capsule
 * The Sprite component (inherited from APaperCharacter) handles the visuals
 * config=Game
 */
UCLASS()
class ARPGCharacterBase : public APaperCharacter, public IInteraction
{
	GENERATED_BODY()

	

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
public:
	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Movement")
	UPaperFlipbook* IdleAnimation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Fight")
	UPaperFlipbook *AttackAnimation;

	/*Used for animation system(To not accidentally change animation)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Fight")
	bool bAttacking = false;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= "Animations",SaveGame)
	bool bUseAnimationSystem = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Death")
	UPaperFlipbook *DeathAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Montage")
	bool bPlayingAnimMontage = false;

	UFUNCTION(BlueprintCallable,Category = Animation)
	virtual void OnFlipbookFinishedPlaying();

	/*Returns true(and value of length) if started animation false otherwise
	 * Param: Animation - Animation to play
	 * Length - how long anim is
	 */
	UFUNCTION(BlueprintCallable, Category = "Animations|Montage")
	virtual bool PlayFlipbookAnimation(UPaperFlipbook*Animation,float &length);
	
	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation();



	void UpdateCharacter();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=Attack)
	void Attack();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=Interaction)
	void InteractAction();

	

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=Money)
    void AddMoney(int amount);

	UFUNCTION(BlueprintPure,Category= Items)
	virtual FItemInfo GetItemById(int32 id,bool&hasItem);

	UFUNCTION(BlueprintPure,Category= Items)
	virtual FItemInfo GetItemByName(FString devName,bool&hasItem);

	/*Add new item to player's inventory. Never returns false(added for future)*/
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=Items)
	bool AddItem(FItemInfo item);

	/*Remove item from player's inventory.Returns false if item is not found or requested amount is too big*/
	UFUNCTION(BlueprintCallable,Category=Items)
	bool RemoveItem(const FString &devName,int amount);

	/*Set current weapon,armor to be one of the items from inventory. Use this to equip weapons etc.*/
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=Items)
	bool SetCurrentItemById(int id,EItemType type);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=Death)
	void Die();

	/*Returns false if ability can not be used*/
	UFUNCTION(BlueprintCallable)
	virtual bool UseAbility(int id);
	
	/*Returns false if ability can not be used*/
	UFUNCTION(BlueprintCallable)
    virtual bool UseAbilityByName(FString Name);

	/*Adds new ability if it isn't in the array*/
	UFUNCTION(BlueprintCallable)
	virtual bool AddAbility(FAbilityInfo Ability);

	UFUNCTION(BlueprintPure)
	virtual FAbilityInfo GetAbilityInfo(int id,bool&has);

	UFUNCTION(BlueprintPure)
    virtual FAbilityInfo GetAbilityInfoByName(FString name,bool&has);

	/*This function exists only make code look prettier*/
	UFUNCTION(BlueprintPure,Category=Items)
	FItemInfo GetCurrentWeapon(bool &has);

	/*This function exists only make code look prettier*/
	UFUNCTION(BlueprintPure,Category=Items)
    FItemInfo GetCurrentTopPartArmor(bool &has);

	/*This function exists only make code look prettier*/
	UFUNCTION(BlueprintPure,Category=Items)
    FItemInfo GetCurrentBottomPartArmor(bool &has);

	/*This function exists only make code look prettier*/
	UFUNCTION(BlueprintPure,Category=Items)
    FItemInfo GetCurrentMiddleArmor(bool &has);

	
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=Display)
    void UpdatePlayerInfo();
	


	virtual int DealDamage_Implementation(int Damage,AActor*DamageDealer =nullptr, TSubclassOf<ASpecialEffect> SpecialEffect =nullptr) override;

	

	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnQuestFinished OnQuestFinished;

	UPROPERTY(BlueprintAssignable)
	FOnLevelUped OnLevelUped;

	UPROPERTY(BlueprintAssignable)
	FOnDied OnDied;
	
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

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= "Stats|Health",SaveGame)
	bool bDead = false;

	/*The health is amount of hits player can take(techincally some attacks deal more then one damage)*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= "Stats|Health",SaveGame)
	int Health = 5;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= Sound,SaveGame)
	USoundBase *AttackSound  = nullptr;


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


};
