// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Interaction.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "PaperSpriteComponent.h"
#include "PickupableItemBase.generated.h"

UCLASS()
class RPGCORE_API APickupableItemBase : public AActor,public IInteraction
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupableItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	USceneComponent*Scene;
	
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly,Category=Collision)
	UBoxComponent*Collision;

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly,Category=Display)
	UPaperSpriteComponent*Sprite;
	
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly,Category=Display)
	UPaperFlipbookComponent*Flipbook;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item",meta = (ExposeOnSpawn="true"))
	FDataTableRowHandle ItemInfo;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item",meta = (ExposeOnSpawn="true"))
	int Amount = 1;

	/*Call this function if you set itemIfo after creating*/
	void LoadDataFromTable();
	
	virtual void Interact_Implementation(AActor* Interactor) override;
};
