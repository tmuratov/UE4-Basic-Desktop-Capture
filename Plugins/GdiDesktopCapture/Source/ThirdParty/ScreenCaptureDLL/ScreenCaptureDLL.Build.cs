// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class ScreenCaptureDLL : ModuleRules
{
	public ScreenCaptureDLL(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			// Add the import library
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "x64", "Release", "ScreenCaptureDLL.lib"));

			// Delay-load the DLL, so we can load it from the right place first
			PublicDelayLoadDLLs.Add("ScreenCaptureDLL.dll");

			// Ensure that the DLL is staged along with the executable
			//RuntimeDependencies.Add("$(PluginDir)/Binaries/ThirdParty/ScreenCaptureDLL/Win64/ScreenCaptureDLL.dll");
			RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "x64", "Release", "ScreenCaptureDLL.dll"));
        }
		
	}
}
