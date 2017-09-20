// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class EAE_6900EditorTarget : TargetRules
{
	public EAE_6900EditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "EAE_6900" } );
		ExtraModuleNames.AddRange( new string[] { "EAE_6900Editor" } );
	}
}
