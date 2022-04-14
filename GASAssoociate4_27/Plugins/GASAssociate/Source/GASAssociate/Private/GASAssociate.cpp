// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASAssociate.h"

#define LOCTEXT_NAMESPACE "FGASAssociateModule"

void FGASAssociateModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
}

void FGASAssociateModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

}
bool FGASAssociateModule::SupportsDynamicReloading()
{
	return true;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGASAssociateModule, GASAssociate)