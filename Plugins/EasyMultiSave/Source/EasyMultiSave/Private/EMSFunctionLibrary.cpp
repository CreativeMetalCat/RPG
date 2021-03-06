//Easy Multi Save - Copyright (C) 2021 by Michael Hegemann.  

#include "EMSFunctionLibrary.h"

/**
Local Profile
Fully seperate of the other save functions.
**/

bool UEMSFunctionLibrary::SaveLocalProfile(UObject* WorldContextObject)
{
	if (UEMSObject* EMS = UEMSObject::Get(WorldContextObject))
	{
		return EMS->SaveLocalProfile();
	}

	return false;
}

UEMSProfileSaveGame* UEMSFunctionLibrary::GetLocalProfileSaveGame(UObject* WorldContextObject)
{
	if (UEMSObject* EMS = UEMSObject::Get(WorldContextObject))
	{
		return EMS->GetLocalProfileSaveGame();
	}

	return nullptr;
}

/**
Save Game User Profile
**/

void UEMSFunctionLibrary::SetCurrentSaveUserName(UObject* WorldContextObject, const FString& UserName)
{
	if (UEMSObject* EMS = UEMSObject::Get(WorldContextObject))
	{
		EMS->SetCurrentSaveUserName(UserName);
	}
}

/**
Save Slots
**/

void UEMSFunctionLibrary::SetCurrentSaveGameName(UObject* WorldContextObject, const FString & SaveGameName)
{
	if (UEMSObject* EMS = UEMSObject::Get(WorldContextObject))
	{
		EMS->SetCurrentSaveGameName(SaveGameName);
	}
}

TArray<FString> UEMSFunctionLibrary::GetSortedSaveSlots(UObject* WorldContextObject)
{
	if (UEMSObject* EMS = UEMSObject::Get(WorldContextObject))
	{
		return EMS->GetSortedSaveSlots();
	}

	return TArray<FString>();
}

UEMSInfoSaveGame* UEMSFunctionLibrary::GetSlotInfoSaveGame(UObject* WorldContextObject, FString& SaveGameName)
{
	if (UEMSObject* EMS = UEMSObject::Get(WorldContextObject))
	{
		SaveGameName = EMS->GetCurrentSaveGameName();
		return EMS->GetSlotInfoObject();
	}

	return nullptr;
}

UEMSInfoSaveGame* UEMSFunctionLibrary::GetNamedSlotInfo(UObject* WorldContextObject, const FString& SaveGameName)
{
	if (UEMSObject* EMS = UEMSObject::Get(WorldContextObject))
	{
		return EMS->GetSlotInfoObject(SaveGameName);
	}

	return nullptr;
}

bool UEMSFunctionLibrary::DoesSaveSlotExist(UObject* WorldContextObject, const FString& SaveGameName)
{
	if (SaveGameName.IsEmpty())
	{
		return false;
	}

	if (UEMSObject* EMS = UEMSObject::Get(WorldContextObject))
	{
		return EMS->DoesSaveGameExist(SaveGameName);
	}

	return false;
}

/**
Persistent Save Game
**/

bool UEMSFunctionLibrary::SavePersistentObject(UObject* WorldContextObject)
{
	if (UEMSObject* EMS = UEMSObject::Get(WorldContextObject))
	{
		return EMS->SavePersistentObject();
	}

	return false;
}

UEMSPersistentSaveGame* UEMSFunctionLibrary::GetPersistentSave(UObject* WorldContextObject)
{
	if (UEMSObject* EMS = UEMSObject::Get(WorldContextObject))
	{
		return EMS->GetPersistentSave();
	}

	return nullptr;
}

/**
File System
**/

void UEMSFunctionLibrary::DeleteAllSaveDataForSlot(UObject* WorldContextObject, const FString& SaveGameName)
{
	if (UEMSObject* EMS = UEMSObject::Get(WorldContextObject))
	{
		EMS->DeleteAllSaveDataForSlot(SaveGameName);
	}
}

/**
Thumbnail Saving
Simple saving as .png from a 2d scene capture render target source.
**/

UTexture2D* UEMSFunctionLibrary::ImportSaveThumbnail(UObject* WorldContextObject, const FString& SaveGameName)
{
	if (UEMSObject* EMS = UEMSObject::Get(WorldContextObject))
	{
		return EMS->ImportSaveThumbnail(SaveGameName);
	}	

	return nullptr;
}

void UEMSFunctionLibrary::ExportSaveThumbnail(UObject* WorldContextObject, UTextureRenderTarget2D* TextureRenderTarget, const FString& SaveGameName)
{
	if (UEMSObject* EMS = UEMSObject::Get(WorldContextObject))
	{
		return EMS->ExportSaveThumbnail(TextureRenderTarget, SaveGameName);
	}
}

/**
Other Functions
**/

void UEMSFunctionLibrary::SetActorSaveProperties(UObject* WorldContextObject, bool bSkipSave, bool bPersistent)
{
	AActor* SaveActor = Cast<AActor>(WorldContextObject);
	if (SaveActor)
	{
		if (bSkipSave)
		{
			SaveActor->Tags.Add(SkipSaveTag);
		}
		else
		{
			SaveActor->Tags.Remove(SkipSaveTag);
		}

		if (bPersistent)
		{
			SaveActor->Tags.Add(PersistentTag);
		}
		else
		{
			SaveActor->Tags.Remove(PersistentTag);
		}
	}
}

bool UEMSFunctionLibrary::IsSavingOrLoading(UObject* WorldContextObject)
{
	if (UEMSObject* EMS = UEMSObject::Get(WorldContextObject))
	{
		return EMS->IsAsyncSaveOrLoadTaskActive(ESaveGameMode::MODE_All);
	}

	return false;
}
