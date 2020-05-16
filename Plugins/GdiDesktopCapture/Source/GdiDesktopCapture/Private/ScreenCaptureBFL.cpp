// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenCaptureBFL.h"
#include "GdiDesktopCapture.h"

//void UScreenCaptureBFL::UpdateScreenShot() {
//	texture->BeginDestroy();
//	texture = FGdiDesktopCaptureModule::Get().CaptureScreen(this);
//}

UTexture2D* UScreenCaptureBFL::GetTexture() {
	FGdiDesktopCaptureModule::Get().CaptureScreen();
	FGdiDesktopCaptureModule::Get().UpdateTexture();
	return FGdiDesktopCaptureModule::Get().mDynamicTexture;
}

UTexture2D* UScreenCaptureBFL::InitScreenCapture(uint8 numThreads) {
	FGdiDesktopCaptureModule::Get().InitCapture(numThreads);
	return FGdiDesktopCaptureModule::Get().mDynamicTexture;
}

UTexture2D* UScreenCaptureBFL::UpdateTextureAsync() {
	FGdiDesktopCaptureModule::Get().CaptureScreen();
	FGdiDesktopCaptureModule::Get().UpdateTextureAsync();
	return FGdiDesktopCaptureModule::Get().mDynamicTexture;
}

