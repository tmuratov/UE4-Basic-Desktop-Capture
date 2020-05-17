// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenCaptureBFL.h"
#include "GdiDesktopCapture.h"


void UScreenCaptureBFL::UpdateScreenTexture() {
	//FGdiDesktopCaptureModule::Get().CaptureScreen();
	//FGdiDesktopCaptureModule::Get().UpdateTexture();
}

UTexture2D* UScreenCaptureBFL::InitScreenCapture(uint8 numThreads) {
	//FGdiDesktopCaptureModule::Get().InitCapture(numThreads);
	return  nullptr; // FGdiDesktopCaptureModule::Get().mDynamicTexture;
}

void UScreenCaptureBFL::UpdateTextureAsync() {
	//FGdiDesktopCaptureModule::Get().CaptureScreen();
	//FGdiDesktopCaptureModule::Get().UpdateTextureAsync();
}
