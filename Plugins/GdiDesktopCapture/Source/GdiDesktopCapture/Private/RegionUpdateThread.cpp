// Copyright Epic Games, Inc. All Rights Reserved.

#include "RegionUpdateThread.h"
#include "ImageUtils.h"
#include "GdiDesktopCapture.h"


FCaptureThread::FCaptureThread()
{
	Stopping = false;
	DataUpdateThread = FRunnableThread::Create(this, TEXT("DataCap"), 128 * 1024, TPri_Normal, 0);
}

FCaptureThread::~FCaptureThread()
{
	if (DataUpdateThread != nullptr)
	{
		DataUpdateThread->Kill(true);
		delete DataUpdateThread;
		DataUpdateThread = nullptr;
	}
}

bool FCaptureThread::Init() {
	if (!DataUpdateThread) return false;
	else return true;
}

uint32 FCaptureThread::Run() {
	//return if not finished previous task
	while (!Stopping) {
		FGdiDesktopCaptureModule::Get().CaptureScreen();
		GEngine->AddOnScreenDebugMessage(-1, 0.3, FColor::Yellow, "run iter");
		FPlatformProcess::Sleep(1 / 30);
	}
	Stop();
	return 0;
}
void FCaptureThread::Stop() {
	Stopping = true;
}

void FCaptureThread::Exit() {

}




FRegionUpdateThread::FRegionUpdateThread()
{
	Stopping = true;
	HasDataBeenUpdated = false;
	DataUpdateThread = FRunnableThread::Create(this, TEXT("DataUpdater"), 128 * 1024, TPri_SlightlyBelowNormal, 0);
}

FRegionUpdateThread::~FRegionUpdateThread()
{
	if (DataUpdateThread != nullptr)
	{
		DataUpdateThread->Kill(true);
		delete DataUpdateThread;
		DataUpdateThread = nullptr;
	}
	if (region) delete region;
	if (target) target = nullptr;
	if (data) data = nullptr;
}

bool FRegionUpdateThread::Init() {
	return (DataUpdateThread && region && target && data);
}

uint32 FRegionUpdateThread::Run() {
	//return if not finished previous task
	if (!Stopping) return 1;
	Stopping = false;
	if (data) target->UpdateTextureRegions(0, 1, region, pitch, bpp, data);
	Stop();
	return 0;
}
void FRegionUpdateThread::Stop() {
	Stopping = true;
}
void FRegionUpdateThread::Exit() {
	if (DataUpdateThread != nullptr)
	{
		DataUpdateThread->Kill(true);
		delete DataUpdateThread;
		DataUpdateThread = nullptr;
	}
}
