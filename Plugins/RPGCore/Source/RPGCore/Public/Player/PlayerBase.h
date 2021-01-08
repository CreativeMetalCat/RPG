// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


#include "PlayerBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilitySlot1ButtonReleased);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilitySlot2ButtonReleased);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilitySlot3ButtonReleased);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilitySlot4ButtonReleased);

/**
 * Base for all player classes
 */
UCLASS()
class RPGCORE_API APlayerBase : public ARPGCharacterBase
{
	GENERATED_BODY()
protected:
	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess="true"))
	UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
public:
#pragma region AbilitySlotButtonsReleasedEventSetup
	UPROPERTY(BlueprintAssignable)
	FOnAbilitySlot1ButtonReleased OnAbilitySlot1ButtonReleased;

	UPROPERTY(BlueprintAssignable)
	FOnAbilitySlot2ButtonReleased OnAbilitySlot2ButtonReleased;

	UPROPERTY(BlueprintAssignable)
	FOnAbilitySlot3ButtonReleased OnAbilitySlot3ButtonReleased;

	UPROPERTY(BlueprintAssignable)
	FOnAbilitySlot4ButtonReleased OnAbilitySlot4ButtonReleased;
#pragma endregion
	
	/*ALL quests that game has*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Quest,SaveGame)
	TArray<FQuestInfo> Quests;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=LevelUpSystem,SaveGame)
	UDataTable *PlayerLevelInfoTable;

	/* If combo was successful all abilties cooldown values will be decreased by this value*/
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category= "Combo",SaveGame)
	float ComboTimeBonus = 1.f;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Deaht|Sound")
	USoundBase*DeathSound = nullptr;
	
	/*This return COPY of quest info, changing it will NOT affect original info. See: @ChangeQuestInfo for that
	*
	*/
	UFUNCTION(BlueprintPure,Category=Quest)
    FQuestInfo GetQuestInfo(FString devName,bool&hasQuest);

	/*Returns true if player has this quest, EVEN if it's completed*/
	UFUNCTION(BlueprintPure,Category=Quest)
    bool HasQuest(const FString &devName);
	
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


	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=Exp)
    void AddExp(int amount);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=Exp)
    void LevelUp();

	APlayerBase();

	void DodgeRoll();

	
#pragma region AbilitiesInputFunctions
	/*Uses numbers 1-4*/
	void UseAbilityInSlot(int slotId);
	
	//These functions are only used to be binded to input event
	void UseAbility1(){UseAbilityInSlot(1);}

	//These functions are only used to be binded to input event
	void UseAbility2(){UseAbilityInSlot(2);}

	//These functions are only used to be binded to input event
	void UseAbility3(){UseAbilityInSlot(3);}

	//These functions are only used to be binded to input event
	void UseAbility4(){UseAbilityInSlot(4);}

	//This function calls for every event binded to that button
	void ReleaseAbilitySlot1Button(){OnAbilitySlot1ButtonReleased.Broadcast();}

	//This function calls for every event binded to that button
	void ReleaseAbilitySlot2Button(){OnAbilitySlot2ButtonReleased.Broadcast();}

	//This function calls for every event binded to that button
	void ReleaseAbilitySlot3Button(){OnAbilitySlot3ButtonReleased.Broadcast();}

	//This function calls for every event binded to that button
	void ReleaseAbilitySlot4Button(){OnAbilitySlot4ButtonReleased.Broadcast();}

#pragma endregion
	
	/** Called for side to side input */
	void MoveRight(float Value);

	void MoveUp(float Value);

	virtual  void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void BindEventOnAbilityButtonReleased_Implementation(ASpecialEffect*Effect) override;

	virtual void ApplyComboBonus_Implementation() override;

	virtual void Die_Implementation() override;

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE  UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};
