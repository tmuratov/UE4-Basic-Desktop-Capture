// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Engine.h"
#include "ScreenCaptureDLL/ScreenCaptureDLL/ScreenCapture.h"

class FRegionUpdateThread;

class FGdiDesktopCaptureModule : public IModuleInterface
{
public:

	static inline FGdiDesktopCaptureModule& Get() {
		return FModuleManager::LoadModuleChecked<FGdiDesktopCaptureModule>("GdiDesktopCapture");
	}

	UTexture2D* mDynamicTexture = nullptr;
	FUpdateTextureRegion2D* mUpdateTextureRegion = nullptr;
	
	// pixels data obtained from dll
	ScreenCapture::FInputFrameDescription frame;

	uint32 mDataSize, mPixelCount, w, h;

	// frunnables async tex update
	TArray<FRegionUpdateThread *> threads;

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	bool InitHandler(); // called at module startup - WinAPI dll handler init
	bool Dispose();		// winapi handler destroy, at module shutdown

	void InitCapture(uint8 numThreads = 1);		// allocates variables
	void InitThreads(uint8 numThreads);
	void CaptureScreen();	// captures data from dll
	void UpdateTexture();	// updates texture based on current values
	void UpdateTextureAsync();	// updates texture using threads

	void* LibHandle = nullptr;

};