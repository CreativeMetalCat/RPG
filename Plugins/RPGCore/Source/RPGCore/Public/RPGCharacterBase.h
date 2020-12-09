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
	/* The animation to play while running around
	 *This one is for facing up
	 * */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
	class UPaperFlipbook* RunningAnimationUp;

	/* The animation to play while running around
	*This one is for facing down
	* */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
	class UPaperFlipbook* RunningAnimationDown;

	/* The animation to play while running around
	*This one is for facing left
	* */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
	class UPaperFlipbook* RunningAnimationLeft;

	/* The animation to play while running around
	*This one is for facing right
	* */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
	class UPaperFlipbook* RunningAnimationRight;

	UPaperFlipbook* GetRunningAnimation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Interaction)
	UBoxComponent *InteractionCollision;

	/*Collision boxes used for attacking*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attack)
	UBoxComponent *UpperCollision;

	/*Collision boxes used for attacking*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attack)
	UBoxComponent *LowerCollision;
	
	/*Collision boxes used for attacking*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attack)
	UBoxComponent *LeftCollision;

	/*Collision boxes used for attacking*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attack)
	UBoxComponent *RightCollision;
public:
	// The animation to play while idle (standing still)
	// This one is for facing up
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Movement|Idle")
	UPaperFlipbook* IdleAnimationUp;
	
	// The animation to play while idle (standing still)
	// This one is for facing down
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Movement|Idle")
	UPaperFlipbook* IdleAnimationDown;

	// The animation to play while idle (standing still)
	// This one is for facing right
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Movement|Idle")
	UPaperFlipbook* IdleAnimationRight;
	
	// The animation to play while idle (standing still)
	// This one is for facing left
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Movement|Idle")
	UPaperFlipbook* IdleAnimationLeft;

	UPaperFlipbook* GetIdleAnimation();

	/* Animation to play when character attacks
	 * This one is for facing up
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Fight")
	UPaperFlipbook *AttackAnimationUp;

	/* Animation to play when character attacks
	* This one is for facing down
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Fight")
	UPaperFlipbook *AttackAnimationDown;

	/* Animation to play when character attacks
	* This one is for facing right
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Fight")
	UPaperFlipbook *AttackAnimationRight;

	/* Animation to play when character attacks
	* This one is for facing left
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Fight")
	UPaperFlipbook *AttackAnimationLeft;

	UPaperFlipbook *GetAttackAnimation();

	/* Animation to play when character holds shield
	* This one is for facing up
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Shield")
	UPaperFlipbook *ShieldDrawnAnimationUp;

	/* Animation to play when character holds shield
	* This one is for facing down
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Shield")
	UPaperFlipbook *ShieldDrawnAnimationDown;

	/* Animation to play when character holds shield
	* This one is for facing right
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Shield")
	UPaperFlipbook *ShieldDrawnAnimationRight;

	/* Animation to play when character holds shield
	* This one is for facing left
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Shield")
	UPaperFlipbook *ShieldDrawnAnimationLeft;

	UPaperFlipbook*GetShieldDrawnAnimation();

	/* Animation to play when character rolls
	* This one is for facing left
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Dodge")
	UPaperFlipbook* DodgeRollAnimationUp;

	/* Animation to play when character rolls
	* This one is for facing down
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Dodge")
	UPaperFlipbook* DodgeRollAnimationDown;

	/* Animation to play when character rolls
	* This one is for facing right
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Dodge")
	UPaperFlipbook* DodgeRollAnimationRight;

	/* Animation to play when character rolls
	* This one is for facing left
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Dodge")
	UPaperFlipbook* DodgeRollAnimationLeft;

	UPaperFlipbook* GetDodgeRollAnimation();

	/*Used for animation system(To not accidentally change animation)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Fight")
	bool bAttacking = false;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= "Animations",SaveGame)
	bool bUseAnimationSystem = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Death")
	UPaperFlipbook *DeathAnimation;
	
#pragma region AnimMontage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Montage")
	bool bPlayingAnimMontage = false;

	/*This means that no other animation can play until this one is done*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Montage")
	bool bPlayingTopPriorityAnimMontage = false;
#pragma endregion
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Dodge")
	bool bPlayingDodgeRollAnimation= false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DodgeRoll")
	float DodgeRollSpeed = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DodgeRoll|Cooldown")
	float DodgeRollCooldownTime = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DodgeRoll|Cooldown")
	bool bCanDodgeRoll = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DodgeRoll|Cooldown")
	FTimerHandle DodgeRollCooldownTimerHandle;
	
	UFUNCTION(BlueprintCallable,Category = Animation)
	virtual void OnFlipbookFinishedPlaying();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category = "Animations|Dodge")
	void OnFinishedDodgeRoll();
	

	/*Returns true(and value of length) if started animation false otherwise
	 * Param: Animation - Animation to play
	 * bTopPriority - can this animation be overriden(by attack, dodgeroll etc.) anim 
	 * Length - how long anim is
	 */
	UFUNCTION(BlueprintCallable, Category = "Animations|Montage")
	virtual bool PlayFlipbookAnimation(UPaperFlipbook*Animation,bool bTopPriority,float &length);

	/*used to make code simplier
	 * If you want animation to actually play with bUseAnimationSystem being true use PlayFlipbookAnimation
	 */
    void SetAnimation(UPaperFlipbook*animation,bool Loop);
	
	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation();



	void UpdateCharacter();

	void InputAttack(){Attack(1);}
	
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=Attack)
	void Attack(float damageMultiplier);

	UFUNCTION(BlueprintPure)
	virtual bool CanAttack();

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

	/* Adds Health
	 * returns Amount health that was over the limit
	 */
	UFUNCTION(BlueprintCallable,Category="Stats|Health")
	int AddHealth(int AddHealth);
	
	UFUNCTION(BlueprintPure,Category=Abilities)
	int GetAbilitySlotForAbility(int Id)const;
	
	/*Returns false if ability can not be used*/
	UFUNCTION(BlueprintCallable,Category=Abilities)
	virtual bool UseAbility(int id);
	
	/*Returns false if ability can not be used*/
	UFUNCTION(BlueprintCallable,Category=Abilities)
    virtual bool UseAbilityByName(FString Name);

	/*Adds new ability if it isn't in the array*/
	UFUNCTION(BlueprintCallable,Category=Abilities)
	virtual bool AddAbility(FAbilityInfo Ability);

	UFUNCTION(BlueprintPure,Category=Abilities)
	virtual FAbilityInfo GetAbilityInfo(int id,bool&has)const;

	UFUNCTION(BlueprintPure,Category=Abilities)
    virtual FAbilityInfo GetAbilityInfoByName(FString name,bool&has)const;

	/*Returns -1 if no ability is found*/
	UFUNCTION(BlueprintPure,Category=Abilities)
	int GetAbilityId(FString name);

	UFUNCTION(BlueprintCallable,Category="Abilities|Cooldown")
	virtual void FinishCooldownOnAbility(FString devName);
	
	UFUNCTION(BlueprintCallable,Category="Magic")
	void RestoreMagicJuice();
	
	UFUNCTION(BlueprintCallable,Category="Magic")
	int AddMagicJuice(int Amount);

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

	/*This function exists only make code look prettier*/
	UFUNCTION(BlueprintPure,Category=Items)
    FItemInfo GetCurrentShield(bool &has);

	/*Does roll in direction of Direction
	 * This function is mostly for other functions to use
	 */
	UFUNCTION(BlueprintCallable,Category=DodgeRoll)
	void Roll(EDirection Direction);

	UFUNCTION(BlueprintCallable,Category="Items|Shield")
	virtual bool PutUpShield();

	UFUNCTION(BlueprintCallable,Category="Items|Shield")
    virtual bool PutDownShield();

	UFUNCTION(BlueprintCallable,Category="DodgeRoll|Cooldown")
	void EndDodgeRollCooldown();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=Display)
    void UpdatePlayerInfo();

	virtual int DealDamage_Implementation(int Damage,AActor*DamageDealer =nullptr, TSubclassOf<ASpecialEffect> SpecialEffect =nullptr) override;

	virtual void BeginPlay() override;

	virtual void NotifyAboutEffectStart_Implementation(ASpecialEffect* Effect) override;

	//Removes null effects
	void CleanAppliedEffects();

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

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Effects,SaveGame)
	TArray<ASpecialEffect*> CurrentlyAppliedEffects;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= Stats,SaveGame)
	int AttackPower = 1;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= Stats,SaveGame)
	int Defense = 1;
	

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= "Stats|Health",SaveGame)
	bool bDead = false;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= "Stats|Magic",SaveGame)
	int CurrentMagicJuice = 1;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= "Stats|Magic",SaveGame)
	int MaxMagicJuice = 10; 

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly,Category= "Stats|Magic|Restoration",SaveGame)
	FTimerHandle MagicJuiceRestorationTimerHandle;
	
	/*How much magic juice is restored each "magic juice restoration time"*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= "Stats|Magic|Restoration",SaveGame)
	int MagicJuiceRestorationAmount = 1;

	/*How fast magic juice is restored*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= "Stats|Magic|Restoration",SaveGame)
	float MagicJuiceRestorationTime = 1.f;
	
	
	/*The health is amount of hits player can take(techincally some attacks deal more then one damage)*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= "Stats|Health",SaveGame)
	int Health = 100;
	
	/*The max health is max amount of health player can have(changed with level up)*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= "Stats|Health",SaveGame)
	int MaxHealth = 100;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= Sound,SaveGame)
	USoundBase *AttackSound  = nullptr;


	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Abilities,SaveGame)
	TArray<FAbilityInfo>Abilities;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Abilities,SaveGame)
	int Slot1AbilityId = -1;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Abilities,SaveGame)
	int Slot2AbilityId = -1;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Abilities,SaveGame)
	int Slot3AbilityId = -1;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Abilities,SaveGame)
    int Slot4AbilityId = -1;

	/*Every item player has*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Items,SaveGame)
	TArray<FItemInfo>Items;

	/*
	 * To avoid issues with structs(the ones related to creating copies instead of working with originals) id will be used
	 * To get item Use @GetItemById
	 */
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Items|Weapon",SaveGame)
	int32 CurrentWeaponId = -1;

	/*
	* To avoid issues with structs(the ones related to creating copies instead of working with originals) id will be used
	* To get item Use @GetItemById
	*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Items|Armor",SaveGame)
	int32 TopPartArmorItemId = -1;

	/*
	* To avoid issues with structs(the ones related to creating copies instead of working with originals) id will be used
	* To get item Use @GetItemById
	*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Items|Armor",SaveGame)
	int32 MiddlePartArmorItemId = -1;

	/*
	* To avoid issues with structs(the ones related to creating copies instead of working with originals) id will be used
	* To get item Use @GetItemById
	*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Items|Armor",SaveGame)
	int32 BottomPartArmorItemId = -1;
	
	/*
	* To avoid issues with structs(the ones related to creating copies instead of working with originals) id will be used
	* To get item Use @GetItemById
	*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Items|Shield",SaveGame)
	int32 ShieldItemId = -1;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Items|Shield",SaveGame)
	bool bIsShieldPutUp = false;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Movement",SaveGame)
	float DefaultMovementSpeed = 600.f;
	
	ARPGCharacterBase();


};
