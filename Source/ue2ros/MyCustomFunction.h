// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #ifdef _DEBUG  
// #pragma comment(lib,"opencv_world320d.lib") 
// #else
// #pragma comment(lib,"opencv_world320.lib") 
// #endif

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MyCustomFunction.generated.h"

/**
 * 
 */
UCLASS()
class UE2ROS_API UMyCustomFunction : public UObject
{
	GENERATED_BODY()
public:
	// UFUNCTION(BlueprintCallable)
	// static UTexture2D* GenerateTextureFunction(
	// 	TArray<uint8> ByteArray,
	// 	int32 SrcHeight = 2,
	// 	int32 SrcWidth = 2,
	// 	int32 TextureHeight = 512,
	// 	int32 TextureWidth = 512
	// );
	
	UFUNCTION(BlueprintCallable)
	static TArray<uint8> IntegerArrayToByteArray(TArray<int32> IntegerArray);

	// UFUNCTION(BlueprintCallable)
	// void OpenWebCamera(TArray<FString> UrlArray);
	//
	// UFUNCTION(BlueprintCallable)
	// void OpenLocalCamera(TArray<int32> ID);
	//
	// UFUNCTION(BlueprintCallable)
	// void CloseCamera();

	
};
