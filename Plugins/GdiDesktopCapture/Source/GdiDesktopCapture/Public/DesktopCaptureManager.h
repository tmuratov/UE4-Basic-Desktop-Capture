// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine.h"
#include "ScreenCaptureDLL/ScreenCaptureDLL/ScreenCapture.h"
#include "DesktopCaptureManager.generated.h"

class FRegionUpdateThread;
class FCaptureThread;


UCLASS()
class GDIDESKTOPCAPTURE_API ADesktopCaptureManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADesktopCaptureManager();

	UPROPERTY()
	UTexture2D* mDynamicTexture = nullptr;
	//UPROPERTY()
	FUpdateTextureRegion2D* mUpdateTextureRegion = nullptr;

	//pixels data obtained from dll
	ScreenCapture::FInputFrameDescription frame;
	uint32 mDataSize, mPixelCount, w, h;

	// frunnables async tex update
	TArray<FRegionUpdateThread*> threads;
	FCaptureThread* capThread;
	
	TArray<uint8> mDisplayData;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION() bool InitThreads(uint8 numThreads);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		UTexture2D* GetTexture() {
		return mDynamicTexture;
	}
	UFUNCTION(BlueprintCallable) bool InitCapture(uint8 numThreads = 1);
	UFUNCTION(BlueprintCallable) bool StopCapture();
	UFUNCTION(BlueprintCallable) void UpdateTextureSync();
	UFUNCTION(BlueprintCallable) bool CaptureScreenSync();
};
