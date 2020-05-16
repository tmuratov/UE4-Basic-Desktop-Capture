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
	frame.Dispose();
	for (auto ptr : threads) {
		delete ptr;
		ptr = nullptr;
	}
	threads.Empty();	
	mDynamicTexture = nullptr;
	mUpdateTextureRegion = nullptr;
}

bool FGdiDesktopCaptureModule::InitHandler() {
	return ScreenCapture::InitHandler();
}

bool FGdiDesktopCaptureModule::Dispose() {
	return ScreenCapture::DisposeHandler();
}

void FGdiDesktopCaptureModule::InitCapture(uint8 numThreads) {
	if (mUpdateTextureRegion) delete mUpdateTextureRegion;
	
	frame = ScreenCapture::FInputFrameDescription();

	if (ScreenCapture::GetDesktopScreenshot(frame) == false) return;

	w = frame.FrameWidth;
	h = frame.FrameHeight;
	mDynamicTexture = UTexture2D::CreateTransient(w, h, EPixelFormat::PF_B8G8R8A8);
	mDynamicTexture->CompressionSettings = TextureCompressionSettings::TC_HDR;
	mDynamicTexture->SRGB = 0;
	mDynamicTexture->AddToRoot();
	mDynamicTexture->UpdateResource();

	mDataSize = w * h * 4;
	mPixelCount = w * h;
	mDisplayData.Reserve(mDataSize);
	
	FMemory::Memcpy(mDisplayData.GetData(), frame.FrameBuffer, mDataSize);
	//mDisplayData = TArray<uint8>(frame.FrameBuffer, mDataSize);

	mUpdateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, w, h);

	InitThreads(numThreads);

	if (capThread) {
		delete capThread;
		capThread = nullptr;
	}
	capThread = new FCaptureThread();
	capThread->Init();
	//capThread->Run();
}

// assign image region to each thread;
// image region is horizontal slice of size (w, h/numThreads)
void FGdiDesktopCaptureModule::InitThreads(uint8 numThreads) {
	//possible re-init with another numThreads
	if (threads.Num() > 0) {
		for (auto thread : threads){
			thread->Stop();
			thread->Exit();
			delete thread;
			thread = nullptr;
		}
		threads.Empty();
	}
	int offY=0;
	for (int i = 0; i < numThreads; i++) {
		threads.Add(new FRegionUpdateThread());
		threads[i]->region = new FUpdateTextureRegion2D(0, offY, 0, offY, w, h / numThreads);
		threads[i]->target = mDynamicTexture;
		threads[i]->data = mDisplayData.GetData();//frame.FrameBuffer;
		threads[i]->pitch = mDataSize / h;
		threads[i]->bpp = 4;
		threads[i]->Init();
		offY += h / numThreads;
	}

}

void FGdiDesktopCaptureModule::CaptureScreen() {
	if (ScreenCapture::GetDesktopScreenshot(frame) == false)
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "Not Captured");
	FMemory::Memcpy(mDisplayData.GetData(), frame.FrameBuffer, mDataSize);
}

void FGdiDesktopCaptureModule::UpdateTexture() {
	if (threads.Num() == 1) UpdateTextureAsync();
	else mDynamicTexture->UpdateTextureRegions(0, 1, mUpdateTextureRegion, mDataSize / h, 4, mDisplayData.GetData());
}

void FGdiDesktopCaptureModule::UpdateTextureAsync() {
	//update each region async
	for (auto thread : threads) thread->Run();
}


#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGdiDesktopCaptureModule, GdiDesktopCapture)