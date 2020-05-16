// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "HAL/ThreadingBase.h"
#include "HAL/Platform.h"


class FRegionUpdateThread : public FRunnable {
private:

	// Holds the thread that updates the data
	FRunnableThread* DataUpdateThread;

	// Holds a flag indicating that the thread is stopping.
	bool Stopping = false;

public:
	// bool to check if the data has been updated
	FThreadSafeBool HasDataBeenUpdated = false;

	FUpdateTextureRegion2D* region = nullptr;
	UTexture2D* target = nullptr;
	uint8* data = nullptr;
	uint32 pitch = 0, bpp = 4;

	FRegionUpdateThread() ;
	~FRegionUpdateThread() ;

	// FRunnable interface
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

};
