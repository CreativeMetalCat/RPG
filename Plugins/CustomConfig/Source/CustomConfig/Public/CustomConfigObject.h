// Copyright 2019 Lee Ju Sik

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CustomConfigObject.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCustomConfigObjectChangedSignature);

/**
 * CustomConfigObject provides the ability to load and save user configuration files from Blueprints and synchronize settings between objects. 
 */
UCLASS(Config = CustomConfig, Blueprintable)
class CUSTOMCONFIG_API UCustomConfigObject : public UObject
{
	GENERATED_BODY()
	
public:

	// Save the object's Config attribute variables to the Configuration file.
	// If BroadcastSettingsChanged is True, it synchronizes the variables of all objects of the same class and calls the OnSettingsChanged event.
	UFUNCTION(BlueprintCallable, Category = "CustomConfig")
	void SaveSettings(bool bBroadcastSettingsChanged = true);

	// Load the settings in the Configuration file.
	// If BroadcastSettingsChanged is True, it synchronizes the variables of all objects of the same class and calls the OnSettingsChanged event.
	UFUNCTION(BlueprintCallable, Category = "CustomConfig")
	void LoadSettings(bool bBroadcastSettingsChanged = true);

	// Synchronize variables of the same class objects and call the OnSettingsChanged event.
	UFUNCTION(BlueprintCallable, Category = "CustomConfig")
	void BroadcastSettingsChanged();

	// Called when the user saves to the Configuration file or calls the BroadcastSettingsChanged function.
	// The user can reapply the changed settings from this event.
	UPROPERTY(BlueprintAssignable, Category = "CustomConfig")
	FCustomConfigObjectChangedSignature OnSettingsChanged;

};
