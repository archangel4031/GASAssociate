// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class GASAssociateUE5Target : TargetRules
{
	public GASAssociateUE5Target( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
        BuildEnvironment = TargetBuildEnvironment.Shared;
        DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
        CppStandard = CppStandardVersion.Cpp20;
        ExtraModuleNames.Add("GASAssociateUE5");
	}
}
