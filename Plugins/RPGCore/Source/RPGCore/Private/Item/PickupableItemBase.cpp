// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGCore/Public/Item/PickupableItemBase.h"

#include "RPGCharacterBase.h"

// Sets default values
APickupableItemBase::APickupableItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene=CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = Scene;

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetupAttachment(RootComponent);

	Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	Sprite->SetupAttachment(RootComponent);

	Flipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Flipbook"));
	Flipbook->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APickupableItemBase::BeginPlay()
{
	Super::BeginPlay();
	if(ItemInfo.DataTable && !ItemInfo.IsNull())
	{
		const FItemInfo Item = *ItemInfo.GetRow<FItemInfo>("");
		if(Item.bUseSprite)
		{
			Sprite->SetSprite(Item.WorldDisplaySprite);
			Flipbook->SetVisibility(false);
		}
		else
		{
			Flipbook->SetFlipbook(Item.WorldDisplayPaperFlipbook);
			Flipbook->SetLooping(true);
			Sprite->SetVisibility(false);
		}
	}
}

// Called every frame
void APickupableItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupableItemBase::Interact_Implementation(AActor* Interactor)
{
	if(ItemInfo.DataTable && !ItemInfo.IsNull())
	{
		FItemInfo Item = *ItemInfo.GetRow<FItemInfo>("");
		if (ARPGCharacterBase* character = Cast<ARPGCharacterBase>(Interactor))
		{
			for (int i = 0; i < Amount; i++)
			{
				character->AddItem(Item);
			}
			Destroy();
		}
	}
}

