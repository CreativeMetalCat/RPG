// Copyright 2019 Lee Ju Sik

#include "CustomConfigObject.h"
#include "Misc/ConfigCacheIni.h"
#include "UObject/UObjectIterator.h"

void UCustomConfigObject::SaveSettings(bool bBroadcastSettingsChanged /*= true*/)
{
	SaveConfig();

	if (bBroadcastSettingsChanged)
	{
		BroadcastSettingsChanged();
	}
}

void UCustomConfigObject::LoadSettings(bool bBroadcastSettingsChanged /*= true*/)
{
	FString CustomConfigIni;
	FConfigCacheIni::LoadGlobalIniFile(CustomConfigIni, *GetConfigFilename(this), nullptr, true);
	LoadConfig(NULL, NULL, UE4::LCPF_PropagateToChildDefaultObjects, NULL);
	//SaveConfig();

	if (bBroadcastSettingsChanged)
	{
		BroadcastSettingsChanged();
	}
}

void UCustomConfigObject::BroadcastSettingsChanged()
{
	UClass* CurrentClass = GetClass();
	for (TObjectIterator<UCustomConfigObject> It; It; ++It)
	{
		if (UCustomConfigObject* CustomConfigObj = *It)
		{
			if (IsValid(CustomConfigObj) && CustomConfigObj->GetClass() == CurrentClass)
			{
				CustomConfigObj->LoadConfig();
				CustomConfigObj->OnSettingsChanged.Broadcast();
			}
		}
	}
}



