// Copyright 2019 Lee Ju Sik

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FCustomConfigModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
