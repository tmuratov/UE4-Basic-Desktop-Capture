// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ScreenCaptureBFL.generated.h"

/**
 * 
 */

UCLASS(Blueprintable)
class GDIDESKTOPCAPTURE_API UScreenCaptureBFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
private:

	UFUNCTION(Category = "ScreenCapture")
		static void UpdateScreenTexture();
	UFUNCTION(Category = "ScreenCapture")
		static UTexture2D* InitScreenCapture(uint8 numThreads = 1);
	UFUNCTION(Category = "ScreenCapture")
		static void UpdateTextureAsync();
};

