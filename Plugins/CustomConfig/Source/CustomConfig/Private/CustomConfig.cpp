// Copyright 2019 Lee Ju Sik

#include "CustomConfig.h"

#define LOCTEXT_NAMESPACE "FCustomConfigModule"

void FCustomConfigModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FCustomConfigModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCustomConfigModule, CustomConfig)