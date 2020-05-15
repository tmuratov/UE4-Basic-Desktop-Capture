// Copyright Epic Games, Inc. All Rights Reserved.

#include "GdiDesktopCapture.h"
#include "Interfaces/IPluginManager.h"
#include "ScreenCaptureDLL/ScreenCaptureDLL/ScreenCapture.h"
#include "ImageUtils.h"

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

	if (!InitHandler())
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
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	if (LibHandle)
	{
		Dispose();

		FPlatformProcess::FreeDllHandle(LibHandle);
		LibHandle = NULL;
	}
}


bool FGdiDesktopCaptureModule::InitHandler() {
	return ScreenCapture::InitHandler();
}

bool FGdiDesktopCaptureModule::Dispose() {
	return ScreenCapture::DisposeHandler();
}

void FGdiDesktopCaptureModule::InitCapture() {
	if (mUpdateTextureRegion) delete mUpdateTextureRegion;
	mDynamicColors.Empty();

	ScreenCapture::FInputFrameDescription frame;
	if (ScreenCapture::GetDesktopScreenshot(frame) == false) return;

	w = frame.FrameWidth;
	h = frame.FrameHeight;

	mDynamicTexture = UTexture2D::CreateTransient(w, h, EPixelFormat::PF_B8G8R8A8);
	mDynamicTexture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
	mDynamicTexture->SRGB = 0;
	mDynamicTexture->AddToRoot();
	mDynamicTexture->UpdateResource();

	mDataSize = w * h * 4;
	mArraySize = w * h;
	mDynamicColors = TArray<uint8>(frame.FrameBuffer, mDataSize);
	mUpdateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, w, h);

	frame.Dispose();
}

void FGdiDesktopCaptureModule::CaptureScreen() {
	ScreenCapture::FInputFrameDescription frame;
	if (ScreenCapture::GetDesktopScreenshot(frame) == false) return;

	mDynamicColors = TArray<uint8>(frame.FrameBuffer, mDataSize);
	frame.Dispose();
}

void FGdiDesktopCaptureModule::UpdateTexture() {
	mDynamicTexture->UpdateTextureRegions(0, 1, mUpdateTextureRegion, mDataSize / h, 4, mDynamicColors.GetData());
}


#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGdiDesktopCaptureModule, GdiDesktopCapture)