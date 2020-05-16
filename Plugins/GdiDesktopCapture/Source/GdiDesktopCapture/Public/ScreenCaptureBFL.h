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
	//UFUNCTION()
		//static bool UpdateTexture();
public:
	UFUNCTION(BlueprintCallable, Category = "ScreenCapture")
		static void GetTexture();
	UFUNCTION(BlueprintCallable, Category = "ScreenCapture")
		static UTexture2D* InitScreenCapture(uint8 numThreads = 1);
	UFUNCTION(BlueprintCallable, Category = "ScreenCapture")
		static void UpdateTextureAsync();
};


