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

UTexture2D* UScreenCaptureBFL::InitScreenCapture() {

	FGdiDesktopCaptureModule::Get().InitCapture();
	return FGdiDesktopCaptureModule::Get().mDynamicTexture;
}
