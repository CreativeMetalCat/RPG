// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner/SpawnerBase.h"

#include "NavigationSystem.h"
#include "Components/BillboardComponent.h"


// Sets default values
ASpawnerBase::ASpawnerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	RadiusDisplaySphere = CreateDefaultSubobject<USphereComponent>(TEXT("RadiusDisplaySphere"));
	RadiusDisplaySphere->SetupAttachment(RootComponent);
	RadiusDisplaySphere->SetSphereRadius(SpawnRadius);

	EditorBillboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("EditorBillboard"));
	EditorBillboard->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASpawnerBase::BeginPlay()
{
	Super::BeginPlay();
	
	if(ClassToSpawn)//no reason to start spawn timer, if we don't have anything to spawn
	{
		GetWorldTimerManager().SetTimer(SpawnUpdateTimerHandle,this,&ASpawnerBase::UpdateAndSpawn,SpawnRate,true);
	}
	if(GetWorld())
	{
		NavSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());		
	}
}

void ASpawnerBase::OnConstruction(const FTransform& Transform)
{
	RadiusDisplaySphere->SetSphereRadius(SpawnRadius);
}

// Called every frame
void ASpawnerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnerBase::UpdateAndSpawn()
{
	//Spawn location
	FNavLocation location;

	if(NavSystem)
	{
		if(NavSystem != GetWorld()->GetNavigationSystem())
		{
			NavSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
		}
		NavSystem->GetRandomPointInNavigableRadius(GetActorLocation(),SpawnRadius,location);
	
		FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
		GetWorld()->SpawnActor<ARPGCharacterBase>(ClassToSpawn,location.Location,FRotator::ZeroRotator,SpawnParameters);
	}
	
}

