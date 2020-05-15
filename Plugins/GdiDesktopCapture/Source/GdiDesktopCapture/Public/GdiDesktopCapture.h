// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Engine.h"

class FGdiDesktopCaptureModule : public IModuleInterface
{
public:

	static inline FGdiDesktopCaptureModule& Get() {
		return FModuleManager::LoadModuleChecked<FGdiDesktopCaptureModule>("GdiDesktopCapture");
	}

	UTexture2D* mDynamicTexture = nullptr;
	FUpdateTextureRegion2D* mUpdateTextureRegion = nullptr;
	TArray<uint8> mDynamicColors;
	uint32 mDataSize, mArraySize, w, h;

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	bool InitHandler(); // called at module startup - WinAPI dll handler init
	bool Dispose();		// winapi handler destroy, at module shutdown

	void InitCapture();		// allocates variables
	void CaptureScreen();	// captures data from dll
	void UpdateTexture();	// updates texture based on current values

	void* LibHandle = nullptr;

};
