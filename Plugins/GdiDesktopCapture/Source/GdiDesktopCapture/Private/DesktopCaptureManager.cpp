// Fill out your copyright notice in the Description page of Project Settings.

#include "DesktopCaptureManager.h"

#include "GdiDesktopCapture.h"
#include "Interfaces/IPluginManager.h"
#include "ImageUtils.h"
#include "RegionUpdateThread.h"

// Sets default values
ADesktopCaptureManager::ADesktopCaptureManager() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADesktopCaptureManager::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void ADesktopCaptureManager::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}


bool ADesktopCaptureManager::InitCapture(uint8 numThreads) {
	if (mUpdateTextureRegion) delete mUpdateTextureRegion;

	frame = ScreenCapture::FInputFrameDescription();

	if (ScreenCapture::GetDesktopScreenshot(frame) == false) return false;

	w = frame.FrameWidth;
	h = frame.FrameHeight;

	mDynamicTexture = UTexture2D::CreateTransient(w, h, EPixelFormat::PF_B8G8R8A8);
	mDynamicTexture->CompressionSettings = TextureCompressionSettings::TC_HDR;
	mDynamicTexture->SRGB = 0;
	mDynamicTexture->AddToRoot();
	mDynamicTexture->UpdateResource();

	mDataSize = w * h * 4;
	mPixelCount = w * h;
	mDisplayData.Reserve(mDataSize);

	FMemory::Memcpy(mDisplayData.GetData(), frame.FrameBuffer, mDataSize);
	//mDisplayData = TArray<uint8>(frame.FrameBuffer, mDataSize);

	mUpdateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, w, h);

	// threads should be initialized here for async update
	return true; //InitThreads(numThreads);
}

// assign image region to each thread;
// image region is horizontal slice of size (w, h/numThreads)
bool ADesktopCaptureManager::InitThreads(uint8 numThreads) {
	//possible re-init with another numThreads
	if (threads.Num() > 0) {
		for (auto thread : threads) {
			thread->Stop();
			thread->Exit();
			delete thread;
			thread = nullptr;
		}
		threads.Empty();
	}
	int offY = 0;
	for (int i = 0; i < numThreads; i++) {
		threads.Add(new FRegionUpdateThread());
		threads[i]->region = new FUpdateTextureRegion2D(0, offY, 0, offY, w, h / numThreads);
		threads[i]->target = mDynamicTexture;
		threads[i]->data = mDisplayData.GetData();//frame.FrameBuffer;
		threads[i]->pitch = mDataSize / h;
		threads[i]->bpp = 4;
		threads[i]->id = i;
		if (threads[i]->Init()) threads[i]->Run();
		else GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "failed upd thread init");
		offY += h / numThreads;
	}
	if (capThread) {
		delete capThread;
		capThread = nullptr;
	}
	capThread = new FCaptureThread(mDisplayData.GetData());
	if (capThread->Init()) capThread->Run();
	else GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "failed cap thread init");

	return true;
}

bool ADesktopCaptureManager::CaptureScreenSync() {
	if (ScreenCapture::GetDesktopScreenshot(frame) == false) return false;
	else FMemory::Memcpy(mDisplayData.GetData(), frame.FrameBuffer, mDataSize);
	return true;
}

void ADesktopCaptureManager::UpdateTextureSync() {
	if (threads.Num() >= 1) 
		for (auto thread : threads) thread->LoopIteration();
	else 
		mDynamicTexture->UpdateTextureRegions(0, 1, mUpdateTextureRegion, mDataSize / h, 4, mDisplayData.GetData());
}

bool ADesktopCaptureManager::StopCapture() {
	for (auto thread : threads) thread->Stop();
	if (capThread)	capThread->Stop();
	else return false;
	return true;
}
