// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASAssociateHelperCommands.h"

#define LOCTEXT_NAMESPACE "FGASAssociateHelperModule"

void FGASAssociateHelperCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "GASAssociateHelper", "Execute GASAssociateHelper action", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
