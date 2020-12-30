// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/Projectile2dMovementComponent.h"

UProjectile2dMovementComponent::UProjectile2dMovementComponent()
{
	bAutoActivate = true;

	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UProjectile2dMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	
}

void UProjectile2dMovementComponent::BeginPlay()
{
	if(ComponentToMove == nullptr && GetOwner() != nullptr )
	{
		if(GetOwner()->GetRootComponent())
		{
			ComponentToMove = GetOwner()->GetRootComponent();
		}	
	}
	
	CurrentVelocity = InitialVelocity.GetSafeNormal()*MaxSpeed;
	
	SetComponentTickEnabled(true);	
	
	if(UpdateInterval != -1.f)
	{
		SetComponentTickInterval(UpdateInterval);
	}
	GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,IsComponentTickEnabled()?"Will tick":"Won't Tick");

	RegisterComponent();
	
	Super::BeginPlay();
}

void UProjectile2dMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                   FActorComponentTickFunction* ThisTickFunction)
{
	if(TickType == ELevelTick::LEVELTICK_All && ComponentToMove)
	{
		Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

		FQuat NewRotation;
		
		ComponentToMove->MoveComponent(CurrentVelocity*DeltaTime,NewRotation,false);
	}
}
