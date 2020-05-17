// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Engine.h"
#include "ScreenCaptureDLL/ScreenCaptureDLL/ScreenCapture.h"

class FRegionUpdateThread;
class FCaptureThread;

class FGdiDesktopCaptureModule : public IModuleInterface
{
public:

	static inline FGdiDesktopCaptureModule& Get() {
		return FModuleManager::LoadModuleChecked<FGdiDesktopCaptureModule>("GdiDesktopCapture");
	}
	
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	bool InitHandler(); // called at module startup - WinAPI dll handler init
	bool Dispose();		// winapi handler destroy, at module shutdown

	void* LibHandle = nullptr;

};