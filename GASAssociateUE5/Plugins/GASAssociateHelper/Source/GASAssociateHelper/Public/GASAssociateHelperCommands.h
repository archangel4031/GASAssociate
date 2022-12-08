// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "GASAssociateHelperStyle.h"

class FGASAssociateHelperCommands : public TCommands<FGASAssociateHelperCommands>
{
public:

	FGASAssociateHelperCommands()
		: TCommands<FGASAssociateHelperCommands>(TEXT("GASAssociateHelper"), NSLOCTEXT("Contexts", "GASAssociateHelper", "GASAssociateHelper Plugin"), NAME_None, FGASAssociateHelperStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
