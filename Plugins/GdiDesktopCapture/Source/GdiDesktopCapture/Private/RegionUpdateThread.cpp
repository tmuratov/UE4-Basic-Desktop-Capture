// Copyright Epic Games, Inc. All Rights Reserved.

#include "RegionUpdateThread.h"
#include "ImageUtils.h"



	FCaptureThread::FCaptureThread()
	{
		Stopping = true;
		DataUpdateThread = FRunnableThread::Create(this, TEXT("DataCap"), 0, TPri_Normal, 0);
	}
	FCaptureThread::FCaptureThread(uint8* targetPtr)
	{
		target = targetPtr;
		frame = ScreenCapture::FInputFrameDescription();
		//frame.FrameBuffer = target;

		Stopping = true;
		DataUpdateThread = FRunnableThread::Create(this, TEXT("DataCap"), 0, TPri_Normal, 0);
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
		ScreenCapture::DisposeHandler();
		ScreenCapture::InitHandler();
		return true;
	}
	uint32 FCaptureThread::Run() {

		Stopping = false;
		while (!Stopping) {
			if (!ScreenCapture::GetDesktopScreenshot(frame)) {
				GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, "capture failed");
				Stop();
			}
			else FMemory::Memcpy(target, frame.FrameBuffer, frame.FrameHeight * frame.FrameWidth * 4);
			
			FPlatformProcess::Sleep(1);
		}
		Stop();
		return 0;
	}
	void FCaptureThread::Stop() {
		Stopping = true;
	}
	void FCaptureThread::Exit() {

	}


/**Region update thread below**/


FRegionUpdateThread::FRegionUpdateThread()
{
	Stopping = true;
	HasDataBeenUpdated = false;
	DataUpdateThread = FRunnableThread::Create(this, *(FString::Printf((L"DataUpdater%d"), id)), 0, TPri_SlightlyBelowNormal, 0);
}
FRegionUpdateThread::~FRegionUpdateThread()
{
	if (DataUpdateThread != nullptr)
	{
		DataUpdateThread->Kill(true);
		delete DataUpdateThread;
		DataUpdateThread = nullptr;
	}
	if (region) region = nullptr;
	if (target) target = nullptr;
	if (data) data = nullptr;
}

bool FRegionUpdateThread::Init() {
	return (DataUpdateThread && region && target && data);
}
void FRegionUpdateThread::LoopIteration() {
	if (data) target->UpdateTextureRegions(0, 1, region, pitch, bpp, data);
}
uint32 FRegionUpdateThread::Run() {
	//return if not finished previous task
	if (!Stopping) return 1;
	Stopping = false;
	while (!Stopping) {
		LoopIteration();
		FPlatformProcess::Sleep(0.1);
	}
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


