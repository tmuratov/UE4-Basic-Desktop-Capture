// Copyright Epic Games, Inc. All Rights Reserved.

#include "GdiDesktopCapture.h"
#include "Interfaces/IPluginManager.h"
#include "ImageUtils.h"
#include "RegionUpdateThread.h"

#define LOCTEXT_NAMESPACE "FGdiDesktopCaptureModule"

void FGdiDesktopCaptureModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FString DllPath = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("GdiDesktopCapture"))->GetBaseDir(), TEXT("/Source/ThirdParty/ScreenCaptureDLL/x64/Release"));
	FPlatformProcess::PushDllDirectory(*DllPath);
	DllPath = FPaths::Combine(DllPath, TEXT("ScreenCaptureDLL.dll"));

	if (!FPaths::FileExists(DllPath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find the binary folder for the dll. Plug-in will not be functional."));
		return;
	}

	LibHandle = FPlatformProcess::GetDllHandle(*DllPath);
	if (LibHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load required library %s. Plug-in will not be functional."), TEXT("BlackmagicDLL.dll"));
		return;
	}
	if (!ScreenCapture::InitHandler())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to initialize handler. Plug-in will not be functional."));
		return;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Plug-in is initialized and functioning."));
		return;
	}
}

void FGdiDesktopCaptureModule::ShutdownModule()
{
	if (LibHandle)
	{
		ScreenCapture::DisposeHandler();

		FPlatformProcess::FreeDllHandle(LibHandle);
		LibHandle = NULL;
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGdiDesktopCaptureModule, GdiDesktopCapture)