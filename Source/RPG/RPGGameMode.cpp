// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPGGameMode.h"
#include "RPGCharacter.h"

ARPGGameMode::ARPGGameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = ARPGCharacter::StaticClass();	
}
