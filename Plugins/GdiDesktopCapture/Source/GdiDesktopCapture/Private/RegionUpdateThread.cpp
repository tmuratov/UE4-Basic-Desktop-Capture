// Copyright Epic Games, Inc. All Rights Reserved.

#include "RegionUpdateThread.h"
#include "ImageUtils.h"


FRegionUpdateThread::FRegionUpdateThread()
{
	Stopping = true;
	HasDataBeenUpdated = false;
	DataUpdateThread = FRunnableThread::Create(this, TEXT("DataUpdater"), 128 * 1024, TPri_BelowNormal, 0);
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
	else {
		Stop();
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "Data is nullptr!");
	}
	GEngine->AddOnScreenDebugMessage(-1, 0.1, FColor::Blue, "region updated!");
	Stop();
	return 0;
}
void FRegionUpdateThread::Stop() {
	Stopping = true;
}
void FRegionUpdateThread::Exit() { }
